#ifndef StringManip_hpp
#define StringManip_hpp

// CPSC 447 Final Project
// String manipulation functions
// Author: Mark Reggiardo

#include <string>
#include <vector>
using namespace std;

// Splits a string into a vector of substrings each time it sees a separator
// Parameter 1: the string to split
// Parameter 2: The separator to look for
vector<string> splitString(string input, string separator) {
    vector<string> output = vector<string>();

    while (input.find(separator) != string::npos) {
        output.push_back(input.substr(0, input.find(separator)));
        input.erase(0, input.find(separator) + separator.length());
    }

    if (output.size() == 0) {
        output.push_back(input);
    }

    return output;
}

// Combines a vector of strings into a single string with a joining string in between each element
// Parameter 1: The vector to combine into a single string
// Parameter 2: The string to insert in between each element
string combineStrings(vector<string> strings, string joinedBy) {
    string output = "";

    for (int i = 0; i < strings.size(); i++) {
        output += strings[i] + joinedBy;
    }

    return output;
}

// Erases the content of a string until a match is found, does nothing if no match is found
// Parameter 1: The string to erase from
// Parameter 2: The match to find and stop erasing at
// Parameter 3: Wether the matched portion of the string should be erased too
bool eraseStringTillMatch(string &input, string target, bool inclusive) {
    int position = input.find(target);
    int targetLength = inclusive ? target.length() : 0;

    if (position != string::npos and position + targetLength <= input.length()) {
        input.erase(0, position + targetLength);
        return true;
    } else {
        return false;
    }
}

// Returns a string containing only the characters up till a match
// parameter 1: The string to create a substring from
// Parameter 2: The target to find and create a substring until
string getSubstringTillMatch(string input, string target) {
    int position = input.find(target);

    if (position != string::npos) {
        return input.substr(0, position);
    } else {
        return input;
    }
}

#endif