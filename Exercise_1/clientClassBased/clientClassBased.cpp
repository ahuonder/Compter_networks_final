#include "../ClientManager.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Client starting" << endl;

    // Get and display port number to use
    int portNum = getPortNumber(argc, argv, 3001);
    cout << "Using port: " << portNum << endl;

    // Create client manager
    ClientManager client = ClientManager(portNum, 1024, "localhost");
    
    // Get the message to send from the user
    cout << "Enter message to send to server:" << endl;
    client.loadInputIntoBuffer();

    // Send the message, store result length, handle any errors
    int resultLength = client.sendMessage();
    if (resultLength < 0) { error("ERROR writing to socket"); }
        
    // Receive response from server, store length, handle any errors
    resultLength = client.receiveMessage();
    if (resultLength < 0) { error("ERROR reading from socket"); }
    
    // Output the received response
    cout << "Response: " << client.buffer << endl;
    
    // Close the client socket
    client.close();

    cout << "Client program ending" << endl;
    return 0;
}