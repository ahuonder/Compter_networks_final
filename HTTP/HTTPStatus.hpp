#ifndef HTTPStatus_hpp
#define HTTPStatus_hpp

#include <string>

using namespace std;

struct HTTPStatus {
    public:
    HTTPStatus(int number, string title) {
        this->number = number;
        this->title = title;
    }
    
    HTTPStatus(string line) {        
        int pos = line.find(" ");
        
        if (pos != string::npos and pos + 1 < line.length()) {
            line.erase(0, pos + 1);
        } else {
            throwError("Could not get HTTPStatus (1)");
            return;
        }

        pos = line.find(" ");

        if (pos != string::npos and pos + 1 < line.length())
        {
            try {
                this->number = stoi(line.substr(0, pos));
            } catch(invalid_argument error) {
                throwError("Could not get HTTPStatus (2)");
                return;
            }
            
            line.erase(0, pos + 1);
            this->title = line;
        }
        else
        {
            throwError("Could not get HTTPStatus (3)");
        }
    }
    
    int number;
    string title;
};

#endif