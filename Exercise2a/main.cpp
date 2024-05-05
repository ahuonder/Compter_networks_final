// Exercise 2a
// Write a client program to execute a single HTTP GET on any type of content.
// The requested file will be stored locally.

#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"
#include "../SocketManagement/ClientManager.hpp"
using namespace std;

// Executes a single HTTP GET request to a web server and stores the file it gets in response
// argv[1] is the URL to connect to
int main(int argc, char *argv[]) {
    cout << "HTTP Client starting\n";

    if (argc < 2) {
        cerr << "Please provide a URL to connect to\n";
        return 0;
    }

    // Get the URL to connect to from the arguments
    HTTPRequest request = HTTPRequest(argv[1]);

    try {
        // Create client manager to operate on localhost
        // Use port 80 for HTTP
        ClientManager client = ClientManager(request.host, 80);

        // Display the request that will be sent
        cout << "\nSending Request:\n";
        cout << request.getText();

        // Send message, get response, convert to HTTPResponse object
        string message = client.sendThenReceive(request.getText());
        cout << "MESSAGE SIZE: " << message.size() << endl;
        HTTPResponse response = HTTPResponse::decodeFrom(message);

        // Display information about the response
        cout << "Response Status: " << response.status.number << " - " << response.status.title << endl;
        cout << "Header count: " << response.headers.size() << endl;
        cout << "Body size: " << response.body.size() << " bytes\n";

        // Display response headers
        cout << endl << "Headers:\n\n";
        
        string fileName = "";

        for (HTTPHeader header : response.headers) {
            cout << header.getText() << endl;
            
            if (header.name == "Content-Disposition")  {
                fileName = header.value;
                eraseStringTillMatch(fileName, "\"", true);
                fileName = getSubstringTillMatch(fileName, "\"");
            }
        }

        // Display response body
        cout << endl << "Body:" << endl << endl;
        cout << response.body << endl;
        
        if (!fileName.empty()) {
            cout << "FILENAME: " << fileName << endl;
        }
        
    } catch (string error) {
        // Handle thrown exceptions
        cout << "Error occurred: " << error << endl;
        throwError(error);
    }

    cout << "Client program ending" << endl;
    return 0;
}