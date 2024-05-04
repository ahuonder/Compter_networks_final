#ifndef HTTPHeader_hpp
#define HTTPHeader_hpp

#include "../Tools/ThrowError.hpp"
#include "../Tools/StringManip.hpp"

using namespace std;

class HTTPHeader {
    public:
    HTTPHeader(string name, string value) {
        this->name = name;
        this->value = value;
    }
    
    HTTPHeader(string line) {        
        int pos = line.find(": ");
        
        if (pos == string::npos or pos + 1 >= line.length()) {
            throwError("Could not make header from string");
            return;
        }
        
        this->name = line.substr(0, pos);
        this->value = line.substr(pos + 2, line.length());
    }
    
    string name;
    string value;
    
    string getText() {
        return name + ": " + value + "\r\n";
    }
};

#endif