#include "../ServerManager.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Server starting" << endl;

    // Get and display port number to use
    int portNum = getPortNumber(argc, argv, 3001);
    cout << "Using port: " << portNum << endl;
    
    try {
        // Create server manager
        ServerManager server = ServerManager(portNum);
        
        while (true) {
            // Receive message from client, store length, handle any errors
            string message = server.receive();
            cout << "Message Recieved: " << message << endl;
            
            // Send the message back to the client
            server.send(message);
            cout << "Message returned to client: " << message << endl;
        }
    } catch(string error) {
        cout << "Error occurred: " << error << endl;
    }

    cout << "Server program ending" << endl;
    return 0;
}