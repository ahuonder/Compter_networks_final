#ifndef ClientManager_hpp
#define ClientManager_hpp

#include <string>
#include "SocketManager.hpp"

using namespace std;

// Handles a socket connection for a client
class ClientManager: public SocketManager {
public:
    // Sets up a client manager with the given server host name, port, and buffer size
    // Server host name defaults to an empty string, port defaults to 3001
    ClientManager(string serverHostname = "", int port = 3001, int bufferSize = SocketManager::DEFAULT_BUFFER_SIZE) : SocketManager(port, bufferSize)
    {
        this->serverHostname = serverHostname;
        this->setup();
    }

    // Reads and returns a message from the socket
    string receive() {
        this->clearBuffer();
        read(this->socket, this->buffer, this->bufferSize);
        return this->buffer;
    }

    // Writes a message to the socket, returns false if operation failed
    bool send(string message) {
        return write(this->socket, message.c_str(), this->bufferSize) >= 0;
    }
    
    // Returns the host name of the server
    string getServerHostname() { return this->serverHostname; }
    
    // Sets a new host name for the server to connect to and sets up the socket again
    void setServerHostname(string newHostname) {
        this->serverHostname = newHostname;
        this->setup();
    }

private:
    // Host name of the server to connect to
    string serverHostname;

    // Sets up the socket, starts listening on it, and gets the server info
    void setup() {
        // Get information about the server we want to connect to
        hostent *serverHostInfo = ::gethostbyname(this->serverHostname.c_str());

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
};

#endif