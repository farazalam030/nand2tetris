#include "Parser.h"
#include "flags.h"

bool Parser::hasFurtherCommands() { return !asmFile.eof(); }

void Parser::parseNext(uint64_t &lineNumber) {
  string currLine;
  uint64_t commentPos;
  bool isValidCommand = false;
  while (!isValidCommand && getline(asmFile, currLine)) {
    lineNumber++;
    currLine.erase(remove_if(currLine.begin(), currLine.end(), ::isspace),
                   currLine.end());
    commentPos = currLine.find("//");
    if (commentPos != string::npos) {
      currLine.erase(commentPos, currLine.length() - commentPos);
    }
    isValidCommand = !currLine.empty();
  }
  currentCommand = currLine;
}

char Parser::commandType(uint64_t &lineNumber) {
  if (commandTable.find(currentCommand[0]) != commandTable.end()) {
    return commandTable[currentCommand[0]];
  } else {
    cout << "Invalid Syntax at line " << lineNumber << endl;
    exit(ERROR);
  }
}

string Parser::getSymbolMnemonic() {
  uint64_t openBracketPos, closeBracketPos;

  openBracketPos = currentCommand.find('(');
  closeBracketPos = currentCommand.find(')');

  if (currentCommand[0] == '@') {
    return (
        currentCommand.substr(1, currentCommand.length() - 1)); // A Instruction

  } else if (openBracketPos != string::npos &&
             closeBracketPos != string::npos) {
    return (currentCommand.substr(openBracketPos + 1,
                                  closeBracketPos - openBracketPos - 1));
  }
  return "";
}

string Parser::getDestMnemonic() {
  uint64_t equalSignPosition;
  equalSignPosition = currentCommand.find('=');
  if (equalSignPosition != string::npos) {
    return currentCommand.substr(0, equalSignPosition);
  }
  return "";
}

string Parser::getcompMnemonic() {
  uint64_t equalsignPosition, semicolonPosition;
  equalsignPosition = currentCommand.find('=');
  semicolonPosition = currentCommand.find(';');

  if (equalsignPosition != string::npos) {
    if (semicolonPosition != string::npos) {
      return currentCommand.substr(equalsignPosition + 1,
                                   semicolonPosition - equalsignPosition - 1);
    }
    return currentCommand.substr(
        equalsignPosition + 1, currentCommand.length() - equalsignPosition - 1);
  } else {
    return currentCommand.substr(0, semicolonPosition);
  }

  return "";
}

string Parser::getJumpMnemonic() {
  uint64_t semiColonPosition;
  semiColonPosition = currentCommand.find(';');
  if (semiColonPosition != string::npos) {
    return (
        currentCommand.substr(semiColonPosition + 1,
                              currentCommand.length() - semiColonPosition - 1));
  }
  return "";
}