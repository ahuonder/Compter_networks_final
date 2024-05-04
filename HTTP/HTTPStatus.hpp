#ifndef HTTPStatus_hpp
#define HTTPStatus_hpp

// CPSC 447 Final Project
// A class that contains info for an HTTP status
// Author: Mark Reggiardo

#include <string>
using namespace std;

// Represents an HTTP status
struct HTTPStatus {
    public:
    // Creates an HTTPStatus with a given number and title
    // Parameter 1: The number for the status
    // Parameter 2: The title for the status
    HTTPStatus(int number, string title) {
        this->number = number;
        this->title = title;
    }

    // Decodes an HTTPStatus from a one line string
    // Parameter: The string to decode an HTTP status from (first line of HTTP response)
    // Throws exception if string has invalid format
    static HTTPStatus decodeFrom(string line) {
        // Get rid of protocol information "HTTP/1.1" etc.
        if (!eraseStringTillMatch(line, " ", true)) {
            throwError("Could not get HTTPStatus (1)");
        }
        
        HTTPStatus status = HTTPStatus(-1, "");

        try {
            // Convert the number to an int and save
            status.number = stoi(getSubstringTillMatch(line, " "));
        } catch (invalid_argument error) {
            throwError("Could not get HTTPStatus (2)");
        }

        if (!eraseStringTillMatch(line, " ", true)) {
            throwError("Could not get HTTPStatus (3)");
        }

        status.title = line;
        return status;
    }

    // The numeric code for the status
    int number;
    
    // The name of the status
    string title;

    // Returns a properly formatted representation of the status as a string
    string getText() { 
        return to_string(this->number) + " " + this->title; 
    }
};

#endif