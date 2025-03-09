#pragma once
#include "CommonShared.h"
using namespace std;

const int C_ARITHMETIC = 11;
const int C_PUSH = 1;
const int C_POP = 2;
const int C_LABEL = 3;
const int C_GOTO = 4;
const int C_IF = 5;
const int C_FUNCTION = 6;
const int C_RETURN = 7;
const int C_CALL = 8;
const int COMMENT = 9;
const int C_BLANK = 10;
const int NONE = 0;

class Parser {
private:
  ifstream vmFile;
  string currentCommand, cmd, arg1, arg2;

public:
  int commandType;

  Parser(const string &filename);
  bool hasMoreCommands();
  string advance();
  int getCommandType();

  string argument1();
  int argument2();

  // HELPER Functions
  void setCmdArguments();
  string getCmd();
  string getArg1();
  int getArg2();
  /*TODO HELPER Functions*/
  //   string &getCurrLine();
  //   void close();
  void removeWhitespace(string &line);
  ~Parser();
};