#ifndef ClientManager_hpp
#define ClientManager_hpp

// CPSC 447 Final Project
// A class that manages socket connections for a client
// Author: Mark Reggiardo

#include <string>
#include "SocketManager.hpp"
using namespace std;

// Handles a socket connection for a client
class ClientManager : public SocketManager {
   public:
    // Sets up a client manager
    // Parameter 1: The host name for the server to connect to
    // Parameter 2: The port to connect on, defaults to SocketManager::DEFAULT_PORT
    // Parameter 3: The buffer size, defaults to SocketManager::DEFAULT_BUFFER_SIZE
    // Throws exception if no server host name was provided
    ClientManager(
        string serverHostname, 
        int port = SocketManager::DEFAULT_PORT, 
        int bufferSize = SocketManager::DEFAULT_BUFFER_SIZE
        ) : SocketManager(port, bufferSize) {
            
        if (serverHostname.empty()) {
            throwError("No server hostname provided to ClientManager");
            return;
        }
        
        this->serverHostname = serverHostname;
        this->setup();
    }

    // Receives message from server
    // Returns: Message received from server
    // Throws exception if message was empty or socket could not be read
    string receive() {
        this->clearBuffer();
        int result = read(this->socket, this->buffer, this->bufferSize);

        if (result == 0) {
            throwError("Server disconnected");
            return "";
        } else if (result < 0) {
            throwError("ClientManager receive: failed to read socket");
            return "";
        } else {
            return this->buffer;
        }
    }

    // Sends a message to the server
    // Throws exception if write to socket operation failed
    void send(string message) {
        if (write(this->socket, message.c_str(), message.size()) < 0) {
            throwError("ClientManager send: error writing to socket");
        }
    }

    // Sends a message to the server and then returns its response
    // Throws exception if either operation fails
    string sendThenReceive(string message) {
        this->send(message);
        return this->receive();
    }

    // Returns the host name of the server
    string getServerHostname() { 
        return this->serverHostname; 
    }

   private:
    // Hostname of the server to connect to
    string serverHostname;

    // Sets up the socket, starts listening on it, and gets the server info
    void setup() override {
        // Get information about the server we want to connect to
        hostent *serverHostInfo = ::gethostbyname(this->serverHostname.c_str());

        // Display error if the server we want to connect to cannot be found
        if (serverHostInfo == NULL) {
            throwError(
                "ClientManager ""ructor: could not find server host info "
                "for " +
                this->serverHostname);
        }

        // Set up the server address
        bzero((char *)&(this->address),
              sizeof(this->address));                // Zero out all fields in serverAddress
        this->address.sin_family = AF_INET;          // Listen for internet connections
        this->address.sin_port = htons(this->port);  // Port number in network bye order

        // Copy the server address from server to serverAddress
        bcopy((char *)serverHostInfo->h_addr, (char *)&(this->address).sin_addr.s_addr, serverHostInfo->h_length);

        // Connect to server and display error if connection did not occur
        if (connect(this->socket, (sockaddr *)&(this->address), sizeof(this->address)) < 0) {
            throwError("ClientManager ""ructor: socket file descriptor not found");
        }
    }
};

#endif