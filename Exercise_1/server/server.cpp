#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../shared.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    // Print message indicating server program is staring
    cout << "Server starting" << endl;

    // The port the connection will occur over
    int portNum = getPortNumber(argv, 3001);

    // Output the port that will be used
    cout << "Using port: " << portNum << endl;

    // The length of the input buffer
    int bufferLength = 1024;

    // The input buffer
    char buffer[bufferLength];

    /*
    serverSocketDescriptor: stores the location of socket in the system's file descriptor table
    Parameter 1: make the socket connect to the internet
    Parameter 2: make this socket a stream socket
    Parameter 3: let the system choose the most appropriate connection protocol
    */
    int serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    // If the socketFileDescriptor is bad, display error
    if (serverSocketDescriptor < 0) {
        error("ERROR opening socket");
    }

    // Set up the server address
    sockaddr_in serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));  // Zero out all fields in serverAddress
    serverAddress.sin_family = AF_INET;                     // Listen for internet connections
    serverAddress.sin_addr.s_addr = INADDR_ANY;             // IP address of this machine
    serverAddress.sin_port = htons(portNum);                // Port number in network bye order

    // Bind the server socket to the server address
    if (bind(serverSocketDescriptor, (sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        error("ERROR on binding");
    }

    // Listen for up to 5 incoming connections (max systems usually allow)
    listen(serverSocketDescriptor, 5);

    // Address of the client
    sockaddr_in clientAddress;

    // Size of the client
    socklen_t clientLength = sizeof(clientAddress);

    // Accept incoming connections and store a file descriptor of the client socket
    int clientSocketDescriptor = accept(serverSocketDescriptor, (struct sockaddr *)&clientAddress, &clientLength);

    // Output error if client socket not found
    if (clientSocketDescriptor < 0) {
        error("ERROR on accept");
    }

    // Clear the buffer
    bzero(buffer, bufferLength);

    // Read the message from the client and store the length of the message
    int resultLength = read(clientSocketDescriptor, buffer, bufferLength - 1);

    // Output error if no messsage was found from the client
    if (resultLength < 0) {
        error("ERROR reading from socket");
    }

    // Output the received message
    cout << "Message: " << buffer << endl;

    // Send the message back to the client and store the length of the message
    resultLength = write(clientSocketDescriptor, buffer, resultLength);

    // Output error mesage if writing to socket failed
    if (resultLength < 0) {
        error("ERROR writing to socket");
    }

    // Close the client and server sockets
    close(clientSocketDescriptor);
    close(serverSocketDescriptor);

    return 0;
}