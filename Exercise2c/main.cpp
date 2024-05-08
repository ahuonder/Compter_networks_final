// Exercise 2c
// Write a client program to do threaded parallel HTTP GETs to a Web server.

#include <iostream>
using namespace std;
#include <string.h>
#include <unistd.h>
#include <thread>
#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "../SocketManagement/ClientManager.hpp"
using namespace std;

void single_request(char *URL);
// Executes a single HTTP GET request to a web server
// argv[1] is the URL to connect to
int main(int argc, char* argv[]) {
    cout << "HTTP Client starting\n";

    if (argc < 3) {
        cerr << "Please provide a URL and how many connections to make.\n";
        return 0;
    }

    int num_threads = atoi(argv[2]);
    //thread threads[num_threads];
    vector<thread> threads = vector<thread>();
    
    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(thread(single_request, argv[1]));
        //thread thread_obj = thread(single_request, argv[1]);
        //threads.push_back(move(thread_obj));
        //cout << i << endl;
    }
    
    for (int i = 0; i < num_threads; ++i){
        threads.at(i).join();
    }
    
    
    cout << "Client program ending" << endl;
    return 0;
}

void single_request(char *URL) {
    // Get the URL to connect to from the arguments
    HTTPRequest request = HTTPRequest(URL);

    try {
        // Create client manager to operate on localhost
        // Use port 80 for HTTP
        ClientManager client = ClientManager(request.host, 80);

        // Display the request that will be sent
        cout << "\nSending Request:\n";
        cout << request.getText();

        // Send message, get response, convert to HTTPResponse object
        string message = client.sendThenReceive(request.getText());
        HTTPResponse response = HTTPResponse::decodeFrom(message);

        // Display information about the response
        cout << "Response Status: " << response.status.number << " - " << response.status.title << endl;
        cout << "Header count: " << response.headers.size() << endl;
        cout << "Body size: " << response.body.size() << " bytes\n";

        // Display response headers
        cout << endl << "Headers:\n\n";

        for (HTTPHeader header : response.headers) {
            cout << header.getText() << endl;
        }

        // Display response body
        cout << endl << "Body:" << endl << endl;
        cout << response.body << endl;
    } catch (string error) {
        // Handle thrown exceptions
        cout << "Error occurred: " << error << endl;
        throwError(error);
    }
}