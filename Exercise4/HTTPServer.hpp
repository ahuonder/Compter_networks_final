#ifndef HTTPServer_hpp
#define HTTPServer_hpp

// CPSC 447 Final Project
// A class to implement an HTTP web server that respons with an HTML message
// Author: Mark Reggiardo

#include <fstream>

#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "../SocketManagement/ServerManager.hpp"
using namespace std;

// Overrides ServerManager to perform specic HTTP tasks
class HTTPServer : public ServerManager {
   public:
    // Creates an Echo Server
    // Parameter 1: Buffer size, defualts to SocketManager::DEFAULT_BUFFER_SIZE
    // Parameter 2: Max number of clients that can connect at once, defaults to ServerManager::DEFAULT_CLIENT_CAPACITY
    // Parameter 3: How long to listen for connection events for, defaults to ServerManager::DEFAULT_TIMEOUT
    HTTPServer(int bufferSize = SocketManager::DEFAULT_BUFFER_SIZE,
               int clientCapacity = ServerManager::DEFAULT_CLIENT_CAPACITY,
               int timeout = ServerManager::DEFAULT_TIMEOUT)
        : ServerManager(80, bufferSize, clientCapacity, timeout) {}

   protected:
    // Displays connection message
    // Parameter: The index of the client in the ServerManager
    // Called by ServerManager code when new client connects
    void onConnect(int clientSocket) override {
        cout << "\nHTTPServer New client connected. Connection count now: " << this->getConnectionCount() << endl;
    }

    // Displays disconnect message
    // Parameter 1: The index of the client in the ServerManager
    // Parameter 2: True if client disconnected itself
    // Called by ServerManager code when a client disconnects
    void onDisconnect(int clientSocket, bool didClientDisconnect) override {
        cout << "\nHTTPServer Client disconnected. Connection count now: " << this->getConnectionCount() << endl;
        cout << (didClientDisconnect ? "" : "Not ") << "Disconnected by Client\n";
    }

    // Sends appropriate file to client or 404 error
    // Parameter 1: The message that was received
    // Parameter 2: The index of the client in the ServerManager
    // Called by ServerManager code when a message is received from a client
    void onReceive(string message, int clientSocket) override {
        cout << "\nHTTPServer Message received from client " << clientSocket << endl << endl;
        cout << message << endl;

        HTTPRequest request = HTTPRequest::decodeFrom(message);

        HTTPStatus status = HTTPStatus(200, "OK");
        HTTPResponse response = HTTPResponse(status);
        
        string path = request.path;

        if (path == "/html") {
            response.headers.push_back(HTTPHeader("Content-Disposition", "attachment; filename=\"index.html\""));
            this->send(response.getText(), clientSocket);
            this->send(getFile("index.html", true), clientSocket);
        } else if (path == "/gif") {
            response.headers.push_back(HTTPHeader("Content-Disposition", "attachment; filename=\"snoopy.gif\""));
            this->send(response.getText(), clientSocket);
            this->send(getFile("snoopy.gif", true), clientSocket);
        } else if (path == "/text") {
            response.headers.push_back(HTTPHeader("Content-Disposition", "attachment; filename=\"message.txt\""));
            this->send(response.getText(), clientSocket);
            this->send(getFile("message.txt", true), clientSocket);
        } else {
            response.status = HTTPStatus(404, "NOT FOUND");
            this->send(response.getText(), clientSocket);
        }

        this->removeClient(clientSocket, false);
    }

    // Reads the content of the file at the path as a string
    // Parameter 1: The path name of the file to read
    // Parameter 2: True if new lines should be preserved
    string getFile(string filePath, bool includeNewLines) {
        ifstream file(filePath);
        if (!file.is_open()) {
            return "";
        }

        string output = "";

        while (file.good()) {
            string line = "";
            getline(file, line);
            output += line + (includeNewLines ? "\n" : "");
        }

        file.close();
        return output;
    }
};

#endif