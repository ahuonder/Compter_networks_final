#ifndef ServerManager_hpp
#define ServerManager_hpp

#include "SocketManager.hpp"

using namespace std;

class ServerManager: public SocketManager {
    public:
    sockaddr_in clientAddress;
    socklen_t clientLength;
    int clientSocket;
    
    ServerManager(int port = 3001, int bufferLength = 1024): SocketManager(port, bufferLength) {
        this->setup(); 
    }
    
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
        if (this->clientSocket < 0) {
            error("ERROR on accept");
        }
    }
    
    int receiveMessage() {
        this->clearBuffer();
        return read(this->clientSocket, this->buffer, this->bufferLength - 1);
    }
    
    int sendMessage() {
        return write(this->clientSocket, this->buffer, this->bufferLength);
    }
    
    ~ServerManager() {
        ::close(this->clientSocket);
    }
};

#endif