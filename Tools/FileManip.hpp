#ifndef FileManip_hpp
#define FileManip_hpp

// CPSC 447 Final Project
// File writing and reading functions
// Author: Mark Reggiardo

#include <fstream>
#include <iostream>
#include "ThrowError.hpp"
using namespace std;

// Reads the content of the file at the path as a string
// Parameter 1: The path name of the file to read
string readFileAt(string path) {
    FILE *file = fopen(path.c_str(), "r");

    if (file == NULL) {
        cerr << "Could not read received message from file";
        return 0;
    }

    string message;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    message.resize(size);
    rewind(file);
    
    fread(&message[0], 1, size, file);
    fclose(file);
    return message;
}

// Writes the given string to a file at the given path
// Parameter 1: The string to write to a file
// Parameter 2: The path of the file to write to
// Parameter 3: The number of bytes to write to the file
// Returns: True if successful, false if error
bool writeToFileAt(string text, string path, int size) {
    FILE *file = fopen(path.c_str(), "w");

    if (file == NULL) {
        cerr << "Could not write file received from server";
        return false;
    }

    fwrite(&text[0], 1, size, file);
    fclose(file);
}

// Checks if a file exists at a given path
// Parameter 1: The path to check
// Returns: True if if the file exist
bool fileExistsAt(string path) {
    bool output = false;
    FILE *file = fopen(path.c_str(), "r");
    output = file != NULL;
    fclose(file);
    return output;
}

// Deletes the indicated file
// Parameter: The path of the file to delete
// Returns: True if the file was succesfully deleted
bool deleteFileAt(string path) {
    return remove(path.c_str()) == 0;
}

// Creates a unique name for the file by adding a number to the name if the path already exists
// Parameter 1: The path to check
// Parameter 2: The number of numbers to try to find a unique file name
// Returns: A file name that does not already exists or a blank string if one could not be found
string getUniqueFileNameFor(string path, int tryLimit = 9999) {            
    if (!fileExistsAt(path)) { return path; }
    
    int typePosition = path.rfind(".");
    if (typePosition == string::npos) { return ""; }
    
    string name = path.substr(0, typePosition);
    string type = path.substr(typePosition, path.size());
    
    for (int i = 1; i <= tryLimit; i++) {
        string trialName = name + "-" + to_string(i) + type;
        
        if (!fileExistsAt(trialName)) {
            return trialName;
        }
    }
    
    return "";                   
}

#endif