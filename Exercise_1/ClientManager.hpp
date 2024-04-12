#ifndef ClientManager_hpp
#define ClientManager_hpp

#include <string>
#include "SocketManager.hpp"

using namespace std;

class ClientManager: public SocketManager {
    public:
    const char *serverHostname;
    
    ClientManager(int port = 3001, int bufferLength = 1024, string serverHostname = ""): SocketManager(port, bufferLength) {
        this->serverHostname = serverHostname.c_str();
        this->setup(); 
    }

    void setup() {
        // Get information about the server we want to connect to
        hostent *serverHostInfo = ::gethostbyname(this->serverHostname);

        // Display error if the server we want to connect to cannot be found
        if (serverHostInfo == NULL) {
            error("ERROR, no such host");
        }

        // Set up the server address
        bzero((char *) &(this->address), sizeof(this->address));    // Zero out all fields in serverAddress
        this->address.sin_family = AF_INET;                         // Listen for internet connections
        this->address.sin_port = htons(this->port);                 // Port number in network bye order

        // Copy the server address from server to serverAddress
        bcopy(
            (char *) serverHostInfo->h_addr, 
            (char *) &(this->address).sin_addr.s_addr,
            serverHostInfo->h_length
        );
        
        // Connect to server and display error if connection did not occur
        if (connect(this->socket, (sockaddr *) &(this->address), sizeof(this->address)) < 0) {
            error("ERROR connecting");
        }
    }
    
    int receiveMessage() {
        this->clearBuffer();
        return read(this->socket, this->buffer, this->bufferLength - 1);
    }
    
    int sendMessage() {
        return write(this->socket, this->buffer, this->bufferLength);
    }
};

#endif