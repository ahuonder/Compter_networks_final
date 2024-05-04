#include "../SocketManagement/ClientManager.hpp"
#include "../HTTP/HTTPRequest.hpp"
#include "../HTTP/HTTPResponse.hpp"

using namespace std;

HTTPResponse sendGetRequestAndDisplayResults(HTTPRequest request) {
    try
    {
        // Create client manager to operate on localhost
        // Use port 80 for HTTP
        ClientManager client = ClientManager(request.host, 80);
        
        cout << "Host: " << request.host << ", Path: " << request.path << endl;
        cout << "Request: " << endl
             << request.getText() << endl;

        string message = client.sendThenReceive(request.getText());
        cout << "Message: " << message << endl;
        HTTPResponse response = HTTPResponse(message);

        cout << "Response Status: " << response.status.number << " - " << response.status.title << endl;
        cout << "Header count: " << response.headers.size() << endl;
        cout << "Body size: " << response.body.size() << endl;

        cout << endl
             << "Headers:" << endl
             << endl;

        for (int i = 0; i < response.headers.size(); i++)
        {
            cout << response.headers[i].getText();
        }

        cout << endl
             << "Body:" << endl
             << endl;

        cout << response.body << endl;

        return response;
    }
    catch (string error)
    {
        cout << "Error occurred: " << error << endl;
        throwError(error);
    }
}

int main(int argc, char *argv[]) {
    cout << "HTTP Client starting" << endl;
        
    if (argc < 2) {
        cerr << "Please provide a URL to connect to" << endl;
        return 0;
    }

    // Get the URL to connect to from the arguments
    HTTPRequest request = HTTPRequest(argv[1]);

    try
    {
        // Create client manager to operate on localhost
        // Use port 80 for HTTP
        ClientManager client = ClientManager(request.host, 80);

        cout << "Host: " << request.host << ", Path: " << request.path << endl;
        cout << "Request: " << endl
             << request.getText() << endl;

        string message = client.sendThenReceive(request.getText());
        cout << "Message: " << message << endl;
        HTTPResponse response = HTTPResponse(message);

        cout << "Response Status: " << response.status.number << " - " << response.status.title << endl;
        cout << "Header count: " << response.headers.size() << endl;
        cout << "Body size: " << response.body.size() << endl;

        cout << endl
             << "Headers:" << endl
             << endl;

        for (int i = 0; i < response.headers.size(); i++)
        {
            cout << response.headers[i].getText();
        }

        cout << endl
             << "Body:" << endl
             << endl;

        cout << response.body << endl;
    }
    catch (string error)
    {
        cout << "Error occurred: " << error << endl;
        throwError(error);
    }

    cout << "Client program ending" << endl;
    return 0;
}