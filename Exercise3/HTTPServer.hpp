#ifndef HTTPServer_hpp
#define HTTPServer_hpp

// CPSC 447 Final Project
// A class to implement an HTTP web server that respons with an HTML message
// Author: Mark Reggiardo

#include <fstream>
#include "../SocketManagement/ServerManager.hpp"
#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
using namespace std;

// Overrides ServerManager to perform specic HTTP tasks
class HTTPServer : public ServerManager {
    public:
    // Creates an Echo Server
    // Parameter 1: Buffer size, defualts to SocketManager::DEFAULT_BUFFER_SIZE
    // Parameter 2: Max number of clients that can connect at once, defaults to ServerManager::DEFAULT_CLIENT_CAPACITY
    // Parameter 3: How long to listen for connection events for, defaults to ServerManager::DEFAULT_TIMEOUT
     HTTPServer(
        int bufferSize = SocketManager::DEFAULT_BUFFER_SIZE,
        int clientCapacity = ServerManager::DEFAULT_CLIENT_CAPACITY,
        int timeout = ServerManager::DEFAULT_TIMEOUT
    ) : ServerManager(80, bufferSize, clientCapacity, timeout) {}

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

    // Displays message and echos back to client
    // Parameter 1: The message that was received
    // Parameter 2: The index of the client in the ServerManager
    // Called by ServerManager code when a message is received from a client
    void onReceive(string message, int clientSocket) override {
        cout << "\nHTTPServer Message received from client " << clientSocket << endl << endl;
        cout << message << endl;
        
        HTTPRequest request = HTTPRequest::decodeFrom(message);

        HTTPStatus status = HTTPStatus(200, "OK");
        HTTPResponse response = HTTPResponse(status);
        
        string html = this->getHTML();
        
        if (html != "") {
            response.body = html;
        } else {
            response.status = HTTPStatus(404, "NOT FOUND");
        }
        
        cout << "Sending response:\n";
        cout << response.getText();

        this->send(response.getText(), clientSocket);
    }
    
    // Reads the content of index.html and returns it as a string
    string getHTML() {
        ifstream file("index.html");
        if (!file.is_open()) {
            return "";
        }
        
        string output = "";
        
        while (file.good()) {
            string line = "";
            getline(file, line);
            output += line;
        }
        
        file.close();
        return output;
    }
};

#endif