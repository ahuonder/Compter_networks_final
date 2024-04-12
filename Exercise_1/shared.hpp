#ifndef shared_hpp
#define shared_hpp

#include <string>
using namespace std;

void error(const char *message) {
    perror(message);
    exit(0);
}

/*
    Returns the port number to use for the program
    Parameter 1: The arguments passed into the main function of the program
    Parameter 2: A default port number to use if the user does not provide a valid one
*/
int getPortNumber(int argc, char *argv[], int defaultValue) {
   if (argc > 1) {
      int portNumber = stoi(argv[1]);
      
      if (portNumber >= 0) {
         return portNumber;
      }
   }
   
   return defaultValue;
}

#endif