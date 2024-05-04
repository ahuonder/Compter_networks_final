#ifndef HTTPResponse_hpp
#define HTTPResponse_hpp

#include "HTTPHeader.hpp"
#include "HTTPStatus.hpp"
using namespace std;

class HTTPResponse {
    public:
    HTTPResponse(string message) {
        vector<string> lines = splitString(message, "\r\n");
        
        if (lines.size() < 1) {
            throwError("Not a valid response");
            return;
        }
        
        // Get status
        this->status = HTTPStatus(lines[0]);
                
        // Get headers
        this->headers = vector<HTTPHeader>();
        
        for (int i = 1; i < lines.size(); i++) {
            if (!lines[i].empty()) {
                headers.push_back(HTTPHeader(lines[i]));
            } else {
                if (lines.size() <= i + 1) {
                    this->body = "";
                    return;
                }
                
                lines.erase(lines.begin(), lines.begin() + i + 1);
                break;
            }
        }
        
        this->body = combineStrings(lines, "\r\n");
    }
    
    HTTPStatus status = HTTPStatus(-1, "");
    vector<HTTPHeader> headers;
    string body;
};

#endif