#ifndef ThrowError_hpp
#define ThrowError_hpp

#include <iostream>
using namespace std;

void throwError(string message) {
    cerr << message << endl;
    throw message;
}

#endif