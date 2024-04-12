#include "../ClientManager.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Client starting" << endl;

    // Get and display port number to use
    int portNum = getPortNumber(argc, argv, 3001);
    cout << "Using port: " << portNum << endl;

    try {
        // Create client manager
        ClientManager client = ClientManager("localhost", portNum);

        string message = "";

        while(true) {
            // Get the message to send from the user
            cout << "Enter message to send to server [X to exit program]:" << endl;
            getline(cin, message);

            if (message == "X" or message == "x") { break; }
            
            // Send message and get response, output response
            cout << "Response: " << client.sendMessageAndGetResponse(message) << endl;
        }
    } catch(string error) {
        cout << "Error occurred: " << error << endl;
    }
    
    cout << "Client program ending" << endl;
    return 0;
}