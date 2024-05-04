#ifndef ThrowError_hpp
#define ThrowError_hpp

// CPSC 447 Final Project
// A function that helps handle errors
// Author: Mark Reggiardo

#include <iostream>
using namespace std;

// Displays an error to cerr and throws an error
// Parameter: The message to display and throw
void throwError(string message) {
    cerr << message << endl;
    throw message;
}

#endif