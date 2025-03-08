#include "Parser.h"

std::string ltrim(const std::string &s) {
  std::string result = s;
  result.erase(result.begin(),
               std::find_if(result.begin(), result.end(), [](unsigned char ch) {
                 return !std::isspace(ch);
               }));
  return result;
}

// Trim trailing whitespaces
std::string rtrim(const std::string &s) {
  std::string result = s;
  result.erase(std::find_if(result.rbegin(), result.rend(),
                            [](unsigned char ch) { return !std::isspace(ch); })
                   .base(),
               result.end());
  return result;
}

// Trim both leading and trailing whitespaces
std::string trim(const std::string &s) { return ltrim(rtrim(s)); }

static const set<string> arithCommands = {"add", "sub", "neg", "eq", "gt",
                                          "lt",  "and", "or",  "not"};
static const unordered_map<string, int> commandMap = {
    {"pop", C_POP},       {"push", C_PUSH},         {"if-goto", C_IF},
    {"goto", C_GOTO},     {"function", C_FUNCTION}, {"call", C_CALL},
    {"return", C_RETURN}, {"label", C_LABEL},       {"//", COMMENT}};

Parser::Parser(const string &filename) {
  vmFile.open(filename);
  if (!vmFile.is_open()) {
    cout << "Error opening file " << filename << endl;
  } else {
    cout << "File " << filename << " opened successfully." << endl;
  }

  cmd = "";
  arg1 = "";
  arg2 = "";
  currentCommand = "";
  commandType = NONE;
}

bool Parser::hasMoreCommands() { return vmFile.peek() != EOF; }

string Parser::advance() {
  if (hasMoreCommands()) {

    std::getline(vmFile, currentCommand);
    trim(currentCommand);
    if (currentCommand.length() == 0 ||
        currentCommand[0] == '/' && currentCommand[1] == '/' ||
        currentCommand == "\n") {
      advance();
    }
    setCmdArguments();
    if (currentCommand.length() > 0)
      commandType = getCommandType();
  } else {
    commandType = NONE;
    cout << "Reached end of file." << endl;
  }
  return currentCommand;
}

void Parser::setCmdArguments() {
  string token = "";
  stringstream tokens(currentCommand);
  int i = 0;
  string words[3] = {"", "", ""};
  while (tokens >> token) {
    if (token.length() == 0) {
      continue;
    }
    words[i] = token;
    i++;
  }
  cmd = words[0];
  arg1 = words[1];
  arg2 = words[2];
#ifdef DEBUG
  cout << "In Parser.cpp cmd: " << cmd << " arg1: " << arg1 << " arg2: " << arg2
       << endl;
#endif // DEBUG
}

int Parser::getCommandType() {
#ifdef DEBUG
  cout << "In Parser.cpp getCommandType currentCommand: " << currentCommand
       << endl;
#endif // DEBUG
  currentCommand = trim(currentCommand);
  if (currentCommand[0] == '/' && currentCommand[1] == '/') {
    return COMMENT;
  }
  if (currentCommand.compare("add") == 0 ||
      currentCommand.compare("sub") == 0 ||
      currentCommand.compare("neg") == 0 || currentCommand.compare("eq") == 0 ||
      currentCommand.compare("gt") == 0 || currentCommand.compare("lt") == 0 ||
      currentCommand.compare("and") == 0 || currentCommand.compare("or") == 0 ||
      currentCommand.compare("not") == 0) {
    return C_ARITHMETIC;
  }
  if (currentCommand.find("push") != string::npos) {
    return C_PUSH;
  }
  if (currentCommand.find("pop") != string::npos) {
    return C_POP;
  }
  return NONE;
}

string Parser::argument1() {
  if (commandType == C_ARITHMETIC) {
    return cmd;
  }
  return arg1;
}

int Parser::argument2() { return stoi(arg2); }

Parser::~Parser() {
  vmFile.close();
  vmFile.clear();
}

string Parser::getArg1() { return arg1; }
int Parser::getArg2() { return stoi(arg2); }
string Parser::getCmd() { return cmd; }
