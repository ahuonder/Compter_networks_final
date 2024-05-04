#ifndef HTTPRequest_hpp
#define HTTPRequest_hpp

// Manages information for an HTTPRequest
// Author: Mark Reggiardo

#include "HTTPHeader.hpp"
using namespace std;

// Represents an HTTPRequest
class HTTPRequest {
    public:
    // Create a response to send to another server with a given hostname and path
    // Parameter 1: The hostname of the server to connect to
    // Parameter 2: The path to request on the server
    HTTPRequest(string host, string path) {
        this->host = host;
        this->path = path;
        this->headers = vector<HTTPHeader>();
        this->body = "";
    }

    // Create a response to send to another server with a given url
    // Parameter: The url to connect and request to
    // Throws exception if url has an invalid format
    HTTPRequest(string url) {
        // Get rid of any http information at start of url
        eraseStringTillMatch(url, "https://", true);
        eraseStringTillMatch(url, "http://", true);

        // Get host info and remove from url leaving only the path
        this->host = getSubstringTillMatch(url, "/");

        if (eraseStringTillMatch(url, "/", false)) {
            this->path = url;
        } else {
            this->path = "/";
        }

        // Set the rest of the info
        this->headers = vector<HTTPHeader>();
        this->body = "";
    }

    // Decodes an HTTPRequest from a string
    // Parameter: The string to decode an HTTP request from 
    // Throws exception if string has invalid format
    static HTTPRequest decodeFrom(string message) {
        vector<string> lines = splitString(message, "\r\n");

        if (lines.size() < 2) {
            throwError("Not a valid response");
        }

        HTTPRequest request = HTTPRequest("", "");
        
        // Get request path
        string line = lines[0];
        eraseStringTillMatch(line, " ", true);
        request.path = getSubstringTillMatch(line, " ");
        request.path = getSubstringTillMatch(request.path, "\r\n");

        // Get host
        line = lines[1];
        eraseStringTillMatch(line, ": ", true);
        request.host = getSubstringTillMatch(line, "\r\n");

        // Get headers
        request.headers = HTTPHeader::decodeMultipleFrom(lines);

        // Get body text if a body was sent
        if (lines.size() == 0 or lines[0] != "") {
            request.body = combineStrings(lines, "\r\n");
        }

        return request;
    }

    // Hostname of the server to connect to
    string host;
    
    // Path to request
    string path;
    
    // Headers for request
    vector<HTTPHeader> headers;
    
    // Request body
    string body;

    // Get the properly formatted HTTP protocol text for the request
    string getText() {
        string output = "GET " + this->path + " HTTP/1.1\r\n";
        output += "Host: " + this->host + "\r\n";

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