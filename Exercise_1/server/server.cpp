#include "../EchoServer.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Server starting" << endl;

    // Get and display port number to use
    int portNum = getPortNumber(argc, argv, 3001);
    cout << "Using port: " << portNum << endl;

    try {
        // Create server manager and start listening and echoing messages
        EchoServer echoServer = EchoServer(portNum);
        echoServer.start();
    } catch (string error) {
        cout << "Error occurred: " << error << endl;
    }

    cout << "Server program ending" << endl;
    return 0;
}