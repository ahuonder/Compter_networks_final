#ifndef SocketManager_hpp
#define SocketManager_hpp

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "shared.hpp"

using namespace std;

// A base class to handle socket connections
class SocketManager {
   public:
    // Returns port number of socket
    int getPortNumber() { return this->port; }

    // Sets the socket's port number if the input is >= 0
    void setPortNumber(int newPortNumber) {
        if (newPortNumber >= 0) {
            this->port = newPortNumber;
        }
    }

    // Returns the size of the buffer
    int getBufferSize() { return this->bufferSize; }

    // Clears the buffer and sets its new size the input (or 0 if the input is
    // negative)
    void setNewBufferSize(int newBufferSize) {
        this->clearBuffer();
        this->bufferSize = max(0, newBufferSize);
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

    // Base class constructor to initailze shared functionality across child
    // classes or throws/displays an error
    SocketManager(int port, int bufferSize) {
        this->port = port;
        this->bufferSize = bufferSize;
        this->buffer = new char[bufferSize];

        this->socket = ::socket(AF_INET, SOCK_STREAM, 0);

        if (this->socket < 0) {
            throwError(
                "SocketManager constructor: Could not get socket file descriptor");
        }
    }

    // Overload to setup socket
    virtual void setup(){};

    // Clears the buffer
    void clearBuffer() { bzero(this->buffer, this->bufferSize); }

    // Closes this socket connection
    void close() { ::close(this->socket); }

    // Makes sure the socket is closed and the buffer is cleared and deleted when
    // object is destructed
    ~SocketManager() {
        this->clearBuffer();
        delete this->buffer;
        this->close();
    }
};

#endif