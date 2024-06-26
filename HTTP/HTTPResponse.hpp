#ifndef HTTPResponse_hpp
#define HTTPResponse_hpp

// CPSC 447 Final Project
// Manages information for an HTTPResponse
// Author: Mark Reggiardo

#include "HTTPHeader.hpp"
#include "HTTPStatus.hpp"
using namespace std;

// Represents an HTTP response
class HTTPResponse {
    public:
    // Creates an HTTPReponse with a given status, no headers, and a blank body
    // Parameter: The status of the reponse
    HTTPResponse(HTTPStatus status) {
        this->status = status;
        this->headers = vector<HTTPHeader>();
        this->body = "";
    }

    // Decodes an HTTPResponse from a string
    // Parameter: The string to decode aan HTTP response from
    // Throws exception if string has invalid format
    static HTTPResponse decodeFrom(string message) {
        vector<string> lines = splitString(message, "\r\n");

        if (lines.size() < 1) {
            throwError("Not a valid response");
        }

        HTTPResponse response = HTTPResponse(HTTPStatus::decodeFrom(lines[0]));

        response.headers = HTTPHeader::decodeMultipleFrom(lines);
        
        eraseStringTillMatch(message, "\r\n\r\n", true);
        response.body = message.substr(0, message.size() - 4);
        
        return response;
    }

    // The response status
    HTTPStatus status = HTTPStatus(-1, "");
    
    // The headers fro the repsonse
    vector<HTTPHeader> headers;
    
    // The body of the response
    string body;

    // Get the properly formatted HTTP protocol text for the response
    string getText() {
        string output = "HTTP/1.1 " + this->status.getText() + "\r\n";

        for (HTTPHeader header : this->headers) {
            output += header.getText() + "\r\n";
        }

        if (!this->body.empty()) {
            output += "\r\n" + this->body + "\r\n";
        }

        output += "\r\n";

        return output;
    }
};

#endif