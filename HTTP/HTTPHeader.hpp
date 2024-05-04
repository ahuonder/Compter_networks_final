#ifndef HTTPHeader_hpp
#define HTTPHeader_hpp

// A class that represents an HTTP headeer
// Author: Mark Reggiardo

#include "../Tools/StringManip.hpp"
#include "../Tools/ThrowError.hpp"
using namespace std;

// Represents an HTTP header
class HTTPHeader {
    public:
    // Creates a header
    // Parameter 1: The header's name
    // Parameter 2: The header's value
    HTTPHeader(string name, string value) {
        this->name = name;
        this->value = value;
    }

    // Decodes an HTTPHeader from a one line string
    // Parameter: The string to decode an HTTP header from (from an HTTP request or response)
    // Throws exception if string has invalid format
    static HTTPHeader decodeFrom(string line) {
        HTTPHeader header = HTTPHeader("", "");
        
        header.name = getSubstringTillMatch(line, ": ");

        if (!eraseStringTillMatch(line, ": ", true)) {
            throwError("Could not make header from string");
        }

        header.value = line;
        return header;
    }
    
    // Returns a vector of HTTPHeaders from a given vector of strings
    // Parameter: The vector of strings to decode headers from
    //  - Each string that is turned into an HTTPHeader is removed from the vector that was passed in
    static vector<HTTPHeader> decodeMultipleFrom(vector<string> &lines) {
        vector<HTTPHeader> output = vector<HTTPHeader>();
        
        // Create a header from each line until there is an empty line
        for (int i = 1; i < lines.size(); i++) {
            if (!lines[i].empty()) {
                output.push_back(HTTPHeader::decodeFrom(lines[i]));
            } else {
                int eraseCount = i + ((lines.size() > i + 1) ? 1 : 0);
                lines.erase(lines.begin(), lines.begin() + eraseCount);
                break;
            }
        }
        
        return output;
    }

    // Name of the header
    string name;
    
    // Value of the header
    string value;

    // Returns a properly formatted string represnetaiton of the header
    string getText() { return name + ": " + value; }
};

#endif