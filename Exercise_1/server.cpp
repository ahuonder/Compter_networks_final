#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Server starting..." << endl;
    
    int socketFileDescriptor, newSocketFileDescriptor, portNum, resultLength;
    socklen_t clientLength;
    char buffer[1024];
    
    struct sockaddr_in serv_addr, cli_addr;
    
    int port = 3001;
    if (argc > 1) { port = stoi(argv[1]); }
     
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
     
     
     if (socketFileDescriptor < 0) {
        cout << "ERROR opening socket" << endl;
     }
     
     //Zero out the buffer
     bzero((char *) &serv_addr, sizeof(serv_addr));
          
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);
     
     
    if (bind(socketFileDescriptor, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cout << "ERROR on binding" << endl;
    }
    
     listen(socketFileDescriptor,5);
     
     clientLength = sizeof(cli_addr);
     
     newSocketFileDescriptor = accept(socketFileDescriptor, 
                 (struct sockaddr *) &cli_addr, 
                 &clientLength);
                 
     if (newSocketFileDescriptor < 0) {
          cout << "ERROR on accept" << endl;
     }
     
     bzero(buffer, 1024);
     
     resultLength = read(newSocketFileDescriptor,buffer,1023);
     
     if (resultLength < 0) {
        cout << "ERROR reading from socket" << endl;
     }
     
     cout << "Message: " << buffer << endl;
     
     
     resultLength = write(newSocketFileDescriptor,"I got your message",18);
     
     if (resultLength < 0) {
        cout << "ERROR writing to socket" << endl;
     }
     
     close(newSocketFileDescriptor);
     close(socketFileDescriptor);
     
     return 0;     
}