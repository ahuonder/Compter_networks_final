#ifndef ServerManager_hpp
#define ServerManager_hpp

#include "SocketManager.hpp"

using namespace std;

// Handle a socket connection for a server
class ServerManager: public SocketManager {
public:
    // Sets up a server manager with the given port (defaults to 3001) and buffer size
    ServerManager(int port = 3001, int bufferSize = SocketManager::DEFAULT_BUFFER_SIZE) : SocketManager(port, bufferSize) {
        this->setup();
    }

    // Reads and returns a message from the socket or thorws/displays an error
    string receive() {
        this->clearBuffer();
        int result = read(this->clientSocket, this->buffer, this->bufferSize);
        
        if (result == 0) {
            throwError("ServerManager receive: Received nothing with buffer size 0 from client");
        } if (result < 0) {
            throwError("ServerManager receive: failed to read socket");
            return "";
        } else {
            return this->buffer;
        }
    }

    // Writes a message to the socket or throws/displays an error
    void send(string message) {
        if (write(this->clientSocket, message.c_str(), this->bufferSize) < 0) {
            throwError("ClientManager send: error writing to socket");
        }
    }

    // Returns client address
    sockaddr_in getClientAddress() { return this->clientAddress; }
    
    // Returns client length
    socklen_t getClientLength() { return this->clientLength; }

    // Stop accepting new connections
    void stopAcceptingConnections() {
        ::close(this->clientSocket);
        this->clientSocket = -1;
    }
    
    // Closes the client socket when object is destructed
    ~ServerManager() {
        ::close(this->clientSocket);
    }

private : 
    // Address of the client
    sockaddr_in clientAddress;
    
    // Size of the client's buffer
    socklen_t clientLength;
    
    // File descriptor for client socket
    int clientSocket;

    // Sets up the socket, starts listening on it, and gets the client info or throws/displays an error
    void setup() {
        // Set up the server address
        bzero((char *) &(this->address), sizeof(this->address));  // Zero out all fields in serverAddress
        this->address.sin_family = AF_INET;                     // Listen for internet connections
        this->address.sin_addr.s_addr = INADDR_ANY;             // IP address of this machine
        this->address.sin_port = htons(this->port);             // Port number in network bye order

        // Bind the server socket to the server address or throws/displays an error if binding fails
        if (bind(this->socket, (sockaddr *) &(this->address), sizeof(this->address)) < 0) {
            throwError("ServerManager constructor: could not bind to socket");
        }
        
        // Listen for connections with backlog queue of up to 5 (standard system max.) requests
        listen(this->socket, 5);
        
        this->clientLength = sizeof(clientAddress);
        
        // Connect to server and display error if connection did not occur
        this->clientSocket = accept(this->socket, (sockaddr *)&(this->clientAddress), &(this->clientLength));
        
        if(this->clientSocket < 0) {
            throwError("ServerManager constructor: Failed to connect to client");
        }
    }
};

#endif