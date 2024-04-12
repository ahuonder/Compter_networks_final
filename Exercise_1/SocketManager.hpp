#ifndef SocketManager_hpp
#define SocketManager_hpp

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include "shared.hpp"

using namespace std;

// A base class to handle socket connections
class SocketManager {
public:
    // Overload to setup socket
    virtual void setup(){};

    // Overload to read from socket and return message or throw/display an error
    virtual string receive() { return ""; };

    // Overload to write to socket or throw/display an error
    virtual void send(string message) {};

    // Writes message to socket, then reads message from socket or throws/displays an error
    string sendMessageAndGetResponse(string message) {
        this->send(message);
        return this->receive();
    }

    // Uses send function to write buffer contents to socket or throws/displays an error
    void sendBufferContents() { this->send(this->buffer); }
    
    // Returns port number of socket
    int getPortNumber() { return this->port; }
    
    // Sets the socket's port number if the input is >= 0
    void setPortNumber(int newPortNumber) {
        if (newPortNumber >= 0) {
            this->port = newPortNumber;
        }
    }

    // Clears the buffer 
    void clearBuffer() { bzero(this->buffer, this->bufferSize); }

    // Returns the size of the buffer
    int getBufferSize() { return this->bufferSize; }
    
    // Clears the buffer and sets its new size the input (or 0 if the input is negative)
    void clearBufferAndSetNewSize(int newBufferSize) {
        this->clearBuffer();
        this->bufferSize = max(0, newBufferSize);
    }
    
    // Returns buffer contents
    string getBufferContents() { return this->buffer; }

    // Sets buffer contents to input if that would not overflow the buffer, otherwise returns false
    bool setBufferContents(string newBufferContents) {
        if (newBufferContents.size() > this-> bufferSize) { return false; }
        this->clearBuffer();
        strcpy(this->buffer, newBufferContents.c_str());
        return true;
    }

    // Appends input to buffer contents if that would not overflow the buffer, otherwise returns false
    bool appendToBuffer(string additionalBufferContents) {
        if ((this->buffer + additionalBufferContents).size() > this->bufferSize) { return false; }
        strcat(this->buffer, additionalBufferContents.c_str());
        return true;
    }

    // Adds input to beginning of buffer if that would not overflow the buffer, otherwise returns false
    bool addToBeginningOfBuffer(string additionalBufferContents) {
        if ((this->buffer + additionalBufferContents).size() > this->bufferSize) { return false; }
        strcpy(this->buffer, additionalBufferContents.c_str() + *(this->buffer));
        return true;
    }

    // Get the address of the socket
    sockaddr_in getAddress() { return this->address; }
    
    // Default size for the buffer
    static const int DEFAULT_BUFFER_SIZE = 2048;

protected:
    // The port the socket will be connecting on
    int port;

    // Size of the buffer
    int bufferSize;
    
    // Buffer for storing received information
    char *buffer;
    
    // File descriptor for socket
    int socket;
    
    // Address of socket
    sockaddr_in address;

    // Base class constructor to initailze shared functionality across child classes or throws/displays an error
    SocketManager(int port, int bufferSize) {
        this->port = port;
        this->bufferSize = bufferSize;
        this->buffer = new char[bufferSize];
        this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (this->socket < 0) { throwError("SocketManager constructor: Could not get socket file descriptor"); }
    }

    // Closes this socket connection
    void close() {
        ::close(this->socket);
    }

    // Makes sure the socket is closed and the buffer is cleared and deleted when object is destructed
    ~SocketManager() {
        this->clearBuffer();
        delete this->buffer;
        this->close();
    }
};

#endif