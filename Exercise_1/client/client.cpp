#include "../../SocketManagement/ClientManager.hpp"
#include "../../Tools/GetPortNumber.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Client starting" << endl;

    // Get and display port number to use
    int portNum = getPortNumber(argc, argv, 3001);
    cout << "Using port: " << portNum << endl;

    try {
        // Create client manager to operate on localhost
        ClientManager client = ClientManager("localhost", portNum);

        string message = "";

        while (true) {
            // Get the message to send from the user
            cout << "Enter message to send to server [X to exit program]:" << endl;
            getline(cin, message);
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            
            if (message == "X" or message == "x") {
                break;
            }

            // Send message and get response, output response
            cout << "Response: " << client.sendThenReceive(message) << endl;
        }
    } catch (string error) {
        cout << "Error occurred: " << error << endl;
    }

    cout << "Client program ending" << endl;
    return 0;
}