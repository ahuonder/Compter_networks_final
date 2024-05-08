// CPSC 447 Final Project
// Exercise 1 - Server
// Based on exercise 2.4 from Stalling's textbook
// Create stream-based echo server, which can simultaneously handle multiple clients connecting to it
// Author: Mark Reggiardo
#include <unistd.h>
#include <string.h>
#include "EchoServer.hpp"
using namespace std;

// Runs a stream-based echo server
int main(int argc, char *argv[]) {
    cout << "Server starting" << endl;

    // Get and display port number to use
    int portNum = getPortNumber(argc, argv, 3001);
    cout << "Using port: " << portNum << endl;

    try {
        // Create server manager and start listening and echoing messages
        EchoServer echoServer = EchoServer(portNum);
        echoServer.start();
    } catch (string error) {
        // Handle thrown exceptions
        cout << "Error occurred: " << error << endl;
    }

    cout << "Server program ending" << endl;
    return 0;
}
