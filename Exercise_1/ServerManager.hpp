#ifndef ServerManager_hpp
#define ServerManager_hpp

#include "SocketManager.hpp"

using namespace std;

// Handle a socket connection for a server
class ServerManager: public SocketManager {
public:
    // Sets up a server manager with the given port (defaults to 3001) and buffer size
    ServerManager(int port = 3001, int bufferSize = SocketManager::DEFAULT_BUFFER_SIZE) : SocketManager(port, bufferSize)
    {
        this->setup();
    }

    // Reads and returns a message from the socket
    string receive() {
        this->clearBuffer();
        read(this->clientSocket, this->buffer, this->bufferSize);
        return this->buffer;
    }

    // Writes a message to the socket, returns false if operation failed
    bool send(string message) {
        return write(this->clientSocket, message.c_str(), this->bufferSize) >= 0;
    }

    // Returns client address
    sockaddr_in getClientAddress() { return this->clientAddress; }
    
    // Returns client length
    socklen_t getClientLength() { return this->clientLength; }

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
    
    // Sets up the socket, starts listening on it, and gets the client info
    void setup() {
        // Set up the server address
        bzero((char *) &(this->address), sizeof(this->address));  // Zero out all fields in serverAddress
        this->address.sin_family = AF_INET;                     // Listen for internet connections
        this->address.sin_addr.s_addr = INADDR_ANY;             // IP address of this machine
        this->address.sin_port = htons(this->port);             // Port number in network bye order
        
        // Bind the server socket to the server address
        if (bind(this->socket, (sockaddr *) &(this->address), sizeof(this->address)) < 0) {
            error("ERROR on binding");
        }
        
        // Listen for up to 5 incoming connections (max systems usually allow)
        listen(this->socket, 5);
        
        this->clientLength = sizeof(clientAddress);
        
        // Accept incoming connections and store a file descriptor of the client socket
        this->clientSocket = accept(this->socket, (sockaddr *) &(this->clientAddress), &(this->clientLength));
        
        // Output error if client socket not found
        if (this->clientSocket < 0) { error("ERROR on accept"); }
    }
};

#endif