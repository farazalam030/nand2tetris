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
static const unordered_map<string, Command> commandMap = {
    {"pop", Command::C_POP},           {"push", Command::C_PUSH},
    {"if-goto", Command::C_IF},        {"goto", Command::C_GOTO},
    {"function", Command::C_FUNCTION}, {"call", Command::C_CALL},
    {"return", Command::C_RETURN},     {"label", Command::C_LABEL},
    {"//", Command::COMMENT}};

Parser::Parser(const string &filename) {
  vmFile.open(filename);
  if (!vmFile.is_open()) {
    cout << "Error opening file " << filename << endl;
  } else {
    cout << "File " << filename << " opened successfully." << endl;
  }
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
    commandType = Command::NONE;
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
}

Command Parser::getCommandType() {
  if (currentCommand[0] == '/' && currentCommand[1] == '/') {
    return Command::COMMENT;
  }
  if (arithCommands.find(currentCommand) != arithCommands.end()) {
    return Command::C_ARITHMETIC;
  }
  if (currentCommand.find("push") != string::npos) {
    return Command::C_PUSH;
  }
  if (currentCommand.find("pop") != string::npos) {
    return Command::C_POP;
  }
  return Command::NONE;
}

string Parser::argument1() {
  if (commandType == Command::C_ARITHMETIC) {
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
