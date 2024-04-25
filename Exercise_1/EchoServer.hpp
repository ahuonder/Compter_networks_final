#ifndef EchoServer_hpp
#define EchoServer_hpp

#include "ServerManager.hpp"

using namespace std;

// Handle a socket connection for a server
class EchoServer : public ServerManager {
   public:
    EchoServer(int port = 3001,
               int bufferSize = SocketManager::DEFAULT_BUFFER_SIZE,
               int clientCapacity = ServerManager::DEFAULT_CLIENT_CAPACITY,
               int timeout = ServerManager::DEFAULT_TIMEOUT)
        : ServerManager(port, bufferSize, clientCapacity, timeout) {}

   protected:
    // Called when new client connects
    void onConnect(int clientSocket) override {
        cout << endl << "EchoServer New client connected. Connection count now: "
             << this->getConnectionCount() << endl;
    }

    // Called when a client disconnects
    void onDisconnect(int clientSocket, bool didClientDisconnect) override {
        cout << endl << "EchoServer Client disconnected. Connection count now: "
             << this->getConnectionCount() << endl
             << "Disconnected by client: " << didClientDisconnect << endl;
    }

    // Called when a message is received from a client
    void onReceive(string message, int clientSocket) override {
        cout << endl << "EchoServer onReceive from clientSocketNumber: " << clientSocket << ", message: " << message << endl;
        this->send(message, clientSocket);
    }
};

#endif