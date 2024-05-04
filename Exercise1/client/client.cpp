// CPSC 447 Final Project
// Exercise 1 - Client
// Based on exercise 2.4 from Stalling's textbook
// Create a client to connect to a server
// Author: Mark Reggiardo

#include "../../SocketManagement/ClientManager.hpp"
#include "../../Tools/GetPortNumber.hpp"
using namespace std;

// Creates a client and sends user input to a server
int main(int argc, char *argv[]) {
    cout << "Client starting" << endl;

    // Get and display port number to use
    int portNum = getPortNumber(argc, argv, 3001);
    cout << "Using port: " << portNum << endl;

    try {
        // Create client manager to operate on localhost
        ClientManager client = ClientManager("localhost", portNum);

        string message = "";

        // Allow the user to send multiple messages to the server
        while (true) {
            // Get the message to send from the user
            cout << "Enter message to send to server [X to exit program]:\n";
            getline(cin, message);
            // Keep the cin from exhibiting unexpected behavior with leftover newline character
            cin.clear();
            cin.ignore(INT_MAX, '\n');

            // Stop accepting and sending user input if user chooses to exit program
            if (message == "X" or message == "x") {
                break;
            }

            // Send message and get response, output response
            cout << "Response: " << client.sendThenReceive(message) << endl;
        }
    } catch (string error) {
        // Handle thrown exceptions
        cout << "Error occurred: " << error << endl;
    }

    cout << "Client program ending" << endl;
    return 0;
}