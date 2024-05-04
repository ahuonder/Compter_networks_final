#ifndef StringManip_hpp
#define StringManip_hpp

#include <string>
#include <vector>
using namespace std;

vector<string> splitString(string input, string separator) {
    vector<string> output = vector<string>();

    while (input.find(separator) != string::npos)
    {
        output.push_back(input.substr(0, input.find(separator)));
        input.erase(0, input.find(separator) + separator.length());
    }

    if (output.size() == 0) {
        output.push_back(input);
    }
    
    return output;
}

string combineStrings(vector<string> strings, string joinedBy) {
    string output = "";
    
    for (int i = 0; i < strings.size(); i++) {
        output += strings[i] + joinedBy;
    }
    
    return output;
}

#endif