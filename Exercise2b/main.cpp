// Exercise 2b
// Write a client program to do serial HTTP GETs to a Web server.
// Author: Mark Reggiardo

#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "../SocketManagement/ClientManager.hpp"
using namespace std;

// Executes serial HTTP GET requests to a web server
// argv[1] is the URL to connect to
// argv[2] is the number of times to send the request, defaults to 3
int main(int argc, char *argv[]) {
    cout << "HTTP Client starting\n";

    if (argc < 2) {
        cerr << "Please provide a URL to connect to\n";
        return 0;
    }
    
    int requestCount = 3;
    
    try {
        int arg2 = stoi(argv[2]);
        
        if (arg2 > 0 and arg2 <= 100) {
            requestCount = arg2;
        } else {
            cerr << "Request count should be more than 0 and less than or equal to 100.\n"
                 << arg2 << " is not. Defaulting to " << requestCount << endl;
        }
    } catch (invalid_argument error) {
        cerr << "Could not read number of times to send request, defaulting to " << requestCount << endl;
    }

    // Get the URL to connect to from the arguments
    HTTPRequest request = HTTPRequest(argv[1]);
    cout << "The request will be sent " << requestCount << " times and each request will look like this:\n" << request.getText();

    for (int i = 0; i < requestCount; i++) {
        try {
            // Create client manager to operate on localhost
            // Use port 80 for HTTP
            ClientManager client = ClientManager(request.host, 80);

            // Display the request that will be sent
            cout << "\nSending Request #" << i + 1 << ":\n";

            // Send message, get response, convert to HTTPResponse object
            string message = client.sendThenReceive(request.getText());
            HTTPResponse response = HTTPResponse::decodeFrom(message);

            // Display information about the response
            cout << "Response # " << i + 1 << " Status: " << response.status.number << " - " << response.status.title << endl;
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
        }
    }

    cout << "Client program ending" << endl;
    return 0;
}