#ifndef HTTPServer_hpp
#define HTTPServer_hpp

// CPSC 447 Final Project
// A class to implement an HTTP web server that respons with an HTML message
// Author: Mark Reggiardo

#include <fstream>

#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "../SocketManagement/ServerManager.hpp"
#include "../Tools/FileManip.hpp"
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
        
        // Handle sending the proper data for each requested path or send a 404 error if a valid path was not found
        HTTPResponse response = getResponseForPath(request.path);
        cout << "Sending message with body length of " << response.body.size() << endl;
        this->send(response.getText(), clientSocket);
    }
    
    // Gets the HTTPResponse appropriate for the given path
    // Parameter: The path to get a response for
    // Returns: A response with a 202 OK status code and file data in the body if a file was found from the path
    //          or a 404 NOT FOUND status response if a file was not found or was not read at thae path
    HTTPResponse getResponseForPath(string path) {
        string filePath = "files" + path;
        eraseStringTillMatch(path, "/", true);
        
        HTTPResponse response = HTTPResponse(HTTPStatus::ok);
        
        if (fileExistsAt(filePath)) {
            response.headers.push_back(HTTPHeader("Content-Disposition", "attachment; filename=\"" + path + "\""));
            response.body = readFileAt(filePath);
            
            if (!response.body.empty()) {
                response.headers.push_back(HTTPHeader("Content-Length", to_string(response.body.size())));
            } else {
                response = HTTPResponse(HTTPStatus::notFound);
            }
        } else {
            response = HTTPResponse(HTTPStatus::notFound);
        }
        
        return response;
    }
};

#endif