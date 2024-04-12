#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include "../shared.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    // Print message indicating client program is staring
    cout << "Client starting" << endl;

    // The port the connection will occur over
    int portNum = getPortNumber(argv, 3001);

    // Output the port that will be used
    cout << "Using port: " << portNum << endl;

    // The length of the input buffer
    int bufferLength = 1024;

    // The input buffer
    char buffer[bufferLength];

    /*
    clientSocketDescriptor: stores the location of socket in the system's file descriptor table
    Parameter 1: make the socket connect to the internet
    Parameter 2: make this socket a stream socket
    Parameter 3: let the system choose the most appropriate connection protocol
    */
    int clientSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    // If the clientSocketDescriptor is bad, display error
    if (clientSocketDescriptor < 0) {
        error("ERROR opening socket");
    }
    
    // Get information about the server we want to connect to
    hostent *serverHostInfo = gethostbyname("localhost");

    // Display error if the server we want to connect to cannot be found
    if (serverHostInfo == NULL) {
        error("ERROR, no such host");
    }

    // Set up the server address
    struct sockaddr_in serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));  // Zero out all fields in serverAddress
    serverAddress.sin_family = AF_INET;                     // Listen for internet connections
    serverAddress.sin_port = htons(portNum);                // Port number in network bye order

    // Copy the server address from server to serverAddress
    bcopy(
        (char *) serverHostInfo->h_addr, 
        (char *) &serverAddress.sin_addr.s_addr,
        serverHostInfo->h_length
    );

    // Connect to server and display error if connection did not occur
    if (connect(clientSocketDescriptor, (sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        error("ERROR connecting");
    }

    // Get the message to send from the user
    cout << "Enter message to send to server:" << endl;
    bzero(buffer, bufferLength);
    fgets(buffer, bufferLength - 1, stdin);

    // Send the message and store the result length
    int resultLength = write(clientSocketDescriptor, buffer, strlen(buffer));

    // Display error if message didn't send
    if (resultLength < 0) {
        error("ERROR writing to socket");
    }
    
    // Clear the buffer
    bzero(buffer, bufferLength);
    
    // Read incomming connections and store length of response
    resultLength = read(clientSocketDescriptor, buffer, bufferLength - 1);
    
    // Output error if response not received
    if (resultLength < 0) {
        error("ERROR reading from socket");
    }
    
    // Output the received response
    cout << buffer << endl;
    
    // Close the client socket
    close(clientSocketDescriptor);

    return 0;
}