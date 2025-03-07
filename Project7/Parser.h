#pragma once
#include "CommonShared.h"
using namespace std;

class Parser
{
private:
  ifstream vmFile;
  string currentCommand, cmd, arg1, arg2;

public:
  Command commandType = Command::NONE;

  Parser(const string &filename);
  bool hasMoreCommands();
  string advance();
  Command getCommandType();

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