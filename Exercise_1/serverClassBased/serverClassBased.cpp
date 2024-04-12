#include "../ServerManager.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Server starting" << endl;

    // Get and display port number to use
    int portNum = getPortNumber(argc, argv, 3001);
    cout << "Using port: " << portNum << endl;
    
    // Create server manager
    ServerManager server = ServerManager(portNum, 1024);
    
    // Receive message from client, store length, handle any errors
    int resultLength = server.receiveMessage();
    if (resultLength < 0) { error("ERROR reading from socket"); }

    // Output the received message
    cout << "Message: " << server.buffer << endl;

    // Send the message back to the client, store length of message, handle any errors
    resultLength = server.sendMessage();
    if (resultLength < 0) { error("ERROR writing to socket"); }

    cout << "Server program ending" << endl;
    return 0;
}