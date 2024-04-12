#include "../ServerManager.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Server starting" << endl;

    // Get and display port number to use
    int portNum = getPortNumber(argc, argv, 3001);
    cout << "Using port: " << portNum << endl;
    
    // Create server manager
    ServerManager server = ServerManager(portNum);
    
    // Receive message from client, store length, handle any errors
    string message = server.receive();
    if (message.length() == 0) { error("ERROR reading from socket"); }

    // Output the received message
    cout << "Message: " << message << endl;

    // Send the message back to the client, store length of message, handle any errors
    if (!server.send(message)) { error("ERROR writing to socket"); }

    cout << "Server program ending" << endl;
    return 0;
}