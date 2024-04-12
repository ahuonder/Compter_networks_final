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

class SocketManager {
    public:
    int port;
    int bufferLength;
    char *buffer;
    int socket;
    sockaddr_in address;
    
    virtual void setup() {};
    virtual int receiveMessage() { return -1; };
    virtual int sendMessage() { return -1; };
    
    void loadInputIntoBuffer() {
        this->clearBuffer();
        fgets(this->buffer, this->bufferLength - 1, stdin);
    }
    
    void clearBuffer() {
        bzero(this->buffer, this->bufferLength);
    }
    
    void close() {
        ::close(this->socket);
    }
    
    ~SocketManager() {
        this->close();
        delete this->buffer;
    }
    
    protected:
    SocketManager(int port, int bufferLength) {
        this->port = port;
        this->bufferLength = bufferLength;
        this->buffer = new char[bufferLength];
        this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
        
        if (this->socket < 0) {
            error("ERROR opening socket");
        }
    }
};

#endif