// Exercise 3
// Write a program to accept a connect from a web browser (i.e., acts as an HTTP server) 
// and respond with an HTML message.
// Author: Mark Reggiardo

#include "HTTPServer.hpp"
using namespace std;

/// Runs a stream-based echo server
int main(int argc, char *argv[]) {
    cout << "HTTP Server starting" << endl;

    try {
        // Create server manager and start listening and responding to messages
        HTTPServer server = HTTPServer();
        server.start();
    } catch (string error) {
        // Handle thrown exceptions
        cout << "Error occurred: " << error << endl;
    }

    cout << "Server program ending" << endl;
    return 0;
}