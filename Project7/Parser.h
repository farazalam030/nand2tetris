#pragma once

#include <string>
#include <fstream>
#include <set>
#include <unordered_map>
#include "CommonShared.h"
#include <iostream>
using namespace std;

class Parser
{
private:
    bool failedOpen = false;

    ifstream vmFile;
    string currLine;
    // set<string> calledFunctions;

public:
    Parser(const string &filename)
    {
        vmFile.open(filename);
        int numFunctions = 0;
        if (vmFile.is_open())
        {
            cout << "Opened file " << filename << endl;
        }
        else
        {
            cout << "Error opening file " << filename << " for parsing" << endl;
            failedOpen = true;
        }
    }

    bool getFailedOpen();
    bool hasMoreCommands();
    void advance();
    Command commandType();
    Command currCommandType = Command::NONE;
    string arg1();
    const int arg2();
    string &getCurrLine();
    void close();
    void removeWhitespace(string &line);
    ~Parser();
};