#ifndef EchoServer_hpp
#define EchoServer_hpp

// CPSC 447 Final Project
// A class to implement a stream-based echo server
// Author: Mark Reggiardo

#include "../../SocketManagement/ServerManager.hpp"
#include "../../Tools/GetPortNumber.hpp"
using namespace std;

// Overrides ServerManager to echo received messages back to clients
class EchoServer : public ServerManager {
    public:
    // Creates an Echo Server
    // Parameter 1: Port to use for program, defaults to SocketManager::DEFAULT_PORT
    // Parameter 2: Buffer size, defualts to SocketManager::DEFAULT_BUFFER_SIZE
    // Parameter 3: Max number of clients that can connect at once, defaults to ServerManager::DEFAULT_CLIENT_CAPACITY
    // Parameter 4: How long to listen for connection events for, defaults to ServerManager::DEFAULT_TIMEOUT
    EchoServer(
        int port = SocketManager::DEFAULT_PORT, 
        int bufferSize = SocketManager::DEFAULT_BUFFER_SIZE,
        int clientCapacity = ServerManager::DEFAULT_CLIENT_CAPACITY,
        int timeout = ServerManager::DEFAULT_TIMEOUT
    ) : ServerManager(port, bufferSize, clientCapacity, timeout) {}

    protected:
     // Displays connection message
     // Parameter: The index of the client in the ServerManager
     // Called by ServerManager code when new client connects
     void onConnect(int clientSocket) override {
         cout << "\nEchoServer New client connected. Connection count now: " << this->getConnectionCount() << endl;
    }

    // Displays disconnect message
    // Parameter 1: The index of the client in the ServerManager
    // Parameter 2: True if client disconnected itself
    // Called by ServerManager code when a client disconnects
    void onDisconnect(int clientSocket, bool didClientDisconnect) override {
        cout << "\nEchoServer Client disconnected. Connection count now: " << this->getConnectionCount() << endl;
        cout << (didClientDisconnect ? "" : "Not ") << "Disconnected by Client\n";
    }

    // Displays message and echos back to client
    // Parameter 1: The message that was received
    // Parameter 2: The index of the client in the ServerManager
    // Called by ServerManager code when a message is received from a client
    void onReceive(string message, int clientSocket) override {
        cout << "\nMessage received from client " << clientSocket << endl << endl;
        cout << message << endl;
        this->send(message, clientSocket);
    }
};

#endif