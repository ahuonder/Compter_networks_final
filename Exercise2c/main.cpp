// Exercise 2c
// Write a client program to do threaded parallel HTTP GETs to a Web server.

#include <iostream>
using namespace std;
#include <string.h>
#include <unistd.h>
#include <thread>
#include <semaphore>
#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "../SocketManagement/ClientManager.hpp"
using namespace std;

// Create a semaphore so our threads don't try to access system resources at the exact same time
binary_semaphore semaphore(1);

void single_request(HTTPRequest request, int i);
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

    auto startTime = chrono::high_resolution_clock::now();

    // Get the URL to connect to from the arguments
    HTTPRequest request = HTTPRequest(argv[1]);

    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(thread(single_request, request, i));
    }
    
    for (int i = 0; i < num_threads; ++i){
        threads.at(i).join();
    }

    auto finishTime = chrono::high_resolution_clock::now();
    
    auto elapsedTime = finishTime - startTime;
    
    cout << "Elapsed Time: " << elapsedTime.count() << endl;

    cout << "Client program ending" << endl;
    return 0;
}

void single_request(HTTPRequest request, int i) {
    try {
        // Aquiring the semaphore means only this thread can execute this code until the semaphore is released
        // Otherwise, errors can occur when a large number of threads are created
        semaphore.acquire();
        
        // Create client manager to operate on localhost
        // Use port 80 for HTTP
        ClientManager client = ClientManager(request.host, 80);
        
        // Send message, get response, convert to HTTPResponse object
        string message = client.sendThenReceive(request.getText());

        // We're now done with system resources and can release the lock
        semaphore.release();

        // Decode the response
        HTTPResponse response = HTTPResponse::decodeFrom(message);

        // Output that thread i received a response with a given status
        // We can't use the cout << operator because another thread may write to cout
        // during this process so we compose 1 string to output it all at once, uninterupted
        string outputMessage = "Response received for thread " + to_string(i) + " with status: " + response.status.getText() + "\n";
        cout << outputMessage;
    } catch (string error) {
        // Handle thrown exceptions
        cout << "Error occurred: " << error << endl;
        throwError(error);
    }
}