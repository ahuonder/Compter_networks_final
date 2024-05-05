// Exercise 4
// Write a simple Web server that serves HTML, text, and GIF images.
// Author: Mark Reggiardo

#include <iostream>
using namespace std;

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