#ifndef HTTPRequest_hpp
#define HTTPRequest_hpp

#include "HTTPHeader.hpp"

using namespace std;

class HTTPRequest {
    public:
    HTTPRequest(string host, string path, vector<HTTPHeader> headers = vector<HTTPHeader>()) {
        this->host = host;
        this->path = path;
        this->headers = headers;
    }
    
    HTTPRequest(string url, vector<HTTPHeader> headers = vector<HTTPHeader>()) {
        // Remove http:// or https:// from start of url
        int pos = 0;
    
        if (url.find("https://") != string::npos) { 
            pos = 8;
        } else if (url.find("http://") != string::npos) {
            pos = 7;
        }
        
        url.erase(0, pos);
    
        // Find end of host name and start of path
        pos = url.find("/");
        
        // If end of host name/start of path not found, display error and end
        if (pos == string::npos || pos < 0 || pos > url.length()) {
            throwError("Could not interpret URL");
            return;
        }
            
        this->host = url.substr(0, pos);
        this->path = url.erase(0, pos);
        this->headers = headers;
    }
    
    string host;
    string path;    
    vector<HTTPHeader> headers;
    
    string getText() {
        string output = "GET " + this->path + " HTTP/1.1\r\n";
        output += "Host: " + this->host + "\r\n\r\n";
        return output;
    }    
};

#endif