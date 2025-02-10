
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

using namespace std;

class Parser {
private:
  ifstream asmFile;
  string currentCommand;
  unordered_map<char, char> commandTable;

public:
  Parser(string &fileName) {
    asmFile.open(fileName);
    if (asmFile.fail()) {
      cout << fileName << " NOT FOUND " << endl;
      exit(-1);
    }

    commandTable['@'] = 'A';
    commandTable['A'] = 'C';
    commandTable['D'] = 'C';
    commandTable['M'] = 'C';
    commandTable['0'] = 'C';
    commandTable['1'] = 'C';
    commandTable['-'] = 'C';
    commandTable['!'] = 'C';
    commandTable['('] = 'L';
  }

  bool hasFurtherCommands();
  void parseNext(uint64_t &lineNumber);
  char commandType(uint64_t &lineNumber);
  string getSymbolMnemonic();
  string getDestMnemonic();
  string getcompMnemonic();
  string getJumpMnemonic();
};