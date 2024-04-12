#include "../ClientManager.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Client starting" << endl;

    // Get and display port number to use
    int portNum = getPortNumber(argc, argv, 3001);
    cout << "Using port: " << portNum << endl;

    // Create client manager
    ClientManager client = ClientManager("localhost", portNum);
    
    // Get the message to send from the user
    cout << "Enter message to send to server:" << endl;
    string message = "";
    cin >> message;
    //Send the message and handle if it does not send
    if (!client.send(message)) { error("ERROR writing to socket"); }
        
    // Receive response from server, store length, handle any errors
    message = client.receive();
    if (message.length() == 0) { error("ERROR reading from socket"); }
    
    // Output the received response
    cout << "Response: " << message << endl;

    cout << "Client program ending" << endl;
    return 0;
}