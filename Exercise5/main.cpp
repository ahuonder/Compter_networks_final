// Exercise 5
// Write a program to redirect all GET requests from a browser to a given URL 
// using an HTTP 302 response message to the browser.

#include <iostream>
#include "HTTPServer.hpp"
using namespace std;

int main(int argc, char* argv[]) {
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