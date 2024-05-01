#ifndef ServerManager_hpp
#define ServerManager_hpp

#include <errno.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <vector>
#include "SocketManager.hpp"

using namespace std;

// Handle a socket connection for a server
class ServerManager : public SocketManager {
   public:
    // Sets up a server manager with the given port (defaults to 3001) and buffer
    // size
    ServerManager(int port = 3001,
                  int bufferSize = SocketManager::DEFAULT_BUFFER_SIZE,
                  int clientCapacity = ServerManager::DEFAULT_CLIENT_CAPACITY,
                  int timeout = ServerManager::DEFAULT_TIMEOUT)
        : SocketManager(port, bufferSize) {
        this->clients = new pollfd[clientCapacity]();
        this->timeout = timeout;
        this->setup();
    }

    // Call to start listening to clients and echoing client messages
    void start() { this->getConnectionEvents(); }

    // Writes a message to the socket or throws/displays an error
    void send(string message, int clientIndex) {        
        if (write(this->clients[clientIndex].fd, message.c_str(), message.size()) < 0) {
            throwError("ServerManager send: error writing to socket");
        }
    }

    // Stop accepting new connections
    void stopAcceptingConnections() {
        for (int i = 0; i < this->clientCount; i++) {
            ::close(this->clients[i].fd);
        }

        this->clientCount = 0;
        cout << "No longer accepting connections" << endl;
    }

    // The number of clients currently connected to this server
    int getConnectionCount() {
        if (this->clientCount > 0) {
            return this->clientCount - 1;
        } else {
            return 0;
        }
    }

    // Closes the client socket when object is destructed
    ~ServerManager() {
        this->stopAcceptingConnections();
        delete this->clients;
    }

    // Default maximum number of clients to accept from at once
    static const int DEFAULT_CLIENT_CAPACITY = 128;

    // Default amount of time to poll() for
    static const int DEFAULT_TIMEOUT = 5 * 60 * 1000;

   protected:
    // Override to handle new client connection
    virtual void onConnect(int clientSocket){};

    // Override to handle client disconnections
    virtual void onDisconnect(int clientSocket, bool didClientDisconnect){};

    // Override to handle receiving data from clients
    virtual void onReceive(string message, int clientSocket){};

   private:
    // The number of clients that are currently connected
    int clientCount;

    // An array of clients
    pollfd *clients;

    // How long to poll() for
    int timeout;

    // Sets up the socket, starts listening on it, and gets the client info or
    // throws/displays an error
    void setup() {
        this->clientCount = 0;
        this->connectToSocket();
        this->addClient(this->socket, POLLIN);
    }

    // Add a client when a new connects
    void addClient(int newSocket, int events) {
        this->clients[this->clientCount].fd = newSocket;
        this->clients[this->clientCount].events = events;
        this->clientCount++;
    }

    // Remove a client at the given index when it is no longer in use
    // Second parameter is to indicate if client disconnected itself
    void removeClient(int index, bool didClientDisconnect) {
        ::close(this->clients[index].fd);

        for (int i = index; i < this->clientCount; i++) {
            this->clients[i] = this->clients[i + 1];
        }

        this->clientCount--;
        this->onDisconnect(index, didClientDisconnect);
    }

    // Connect to the socket
    void connectToSocket() {
        int socketOption = 1;

        // Allow program to reuse local address when the server is restarted before
        // the required wait time expires.
        if (setsockopt(this->socket, SOL_SOCKET, SO_REUSEADDR,
                       (char *)&socketOption, sizeof(socketOption)) < 0) {
            cout << "errno: " << errno << endl;
            throwError("setsockopt error");
            return;
        }

        // Set the socket to be nonblocking.
        // All of the sockets for the incoming connections are also nonblocking
        // because they inherit that state from the listening socket.
        if (ioctl(this->socket, FIONBIO, (char *)&socketOption) < 0) {
            throwError("ioctl error");
            return;
        }

        // Set up the server address
        bzero((char *)&(this->address),
              sizeof(this->address));                // Zero out all fields in serverAddress
        this->address.sin_family = AF_INET;          // Listen for internet connections
        this->address.sin_addr.s_addr = INADDR_ANY;  // IP address of this machine
        this->address.sin_port =
            htons(this->port);  // Port number in network bye order

        // Bind to the socket to get its file descriptor
        if (bind(this->socket, (sockaddr *)&(this->address),
                 sizeof(this->address)) < 0) {
            throwError("bind error");
            return;
        }

        // Listen for connections with backlog queue of up to 5 (standard system
        // max.) requests
        listen(this->socket, 5);
    }

    // Accept as many new connections as are available
    void acceptConnections() {
        // Acept all connections
        while (true) {
            // Accept new connection
            int newClientSocket = accept(this->socket, NULL, (socklen_t *)this->bufferSize);

            if (newClientSocket < 0) {
                // Something went wrong if the errno is not EWOULDBLOCK
                // EWOULDBLOCK means we've accepted all of the conenctions
                if (errno != EWOULDBLOCK) {
                    cout << "errno: " << errno << endl;
                    throwError("accept() error");
                }

                break;
            }

            // Add the new client to the list of clients
            this->addClient(newClientSocket, POLLIN);

            // Call overriden method to handle new connection event in child class
            this->onConnect(this->clientCount - 1);
        }
    }

    // Receives data from the client socket at the given index
    void receiveData(int clientIndex) {
        string message = "";

        // Keep receiving data until there's no more to get
        while (true) {
            this->clearBuffer();

            // Receive data on the indicated socket
            int messageLength = read(this->clients[clientIndex].fd, this->buffer,
                                     sizeof(this->buffer));

            // If the messageLength is 0, we're done and we can disconnect. Otherwise, keep receiving
            if (messageLength <= 0) {
                // Client disconnected
                if (messageLength == 0) {
                    this->removeClient(clientIndex, true);
                }

                // Something's wrong if there's an errorno that doesnt equal EWOULDBLOCK
                if (errno != EWOULDBLOCK) {
                    this->removeClient(clientIndex, false);
                    throwError("recv() error");
                }

                // Call overriden onReceive message to handle received data in child class
                this->onReceive(message, clientIndex);
                return;
            } else {
                // Add the data received in this iteration of the loop to our message
                message.append(this->buffer);
            }
        };
    }

    // Gets called by the kernel when the socket has an event occur on it
    // Accepts new connections or receives data based on the type of event
    void getConnectionEvents() {

        while (true) {
            // Allows process to wait for an event to occur and to wake up the process
            // when the event occurs Wait for a timeout            
            // If poll() returns -1, an error has occurred
            if (poll(this->clients, this->clientCount, this->timeout) < 0) {
                throwError("poll error");
                break;
            }

            // Find the clients that need an action performed
            for (int i = 0; i < this->clientCount; i++) {
                short returnedEvents = this->clients[i].revents;

                // If the returned events is 0, move on to the next client
                if (returnedEvents == 0) {
                    continue;
                } else if (returnedEvents != POLLIN) {
                    // If the value is not POLLIN, something weird happened and we should remove this client lol
                    this->removeClient(i, false);
                } else if (this->clients[i].fd == this->socket) {
                    // Accept a new connection
                    this->acceptConnections();
                } else {
                    // Receive from an existing connection
                    this->receiveData(i);
                }
            }
        }
    }
};

#endif