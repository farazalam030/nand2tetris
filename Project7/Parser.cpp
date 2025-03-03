#include "Parser.h"

#include <algorithm>
#include <filesystem>
using namespace std;

static const set<string> arithCommands = {"add", "sub", "neg", "eq", "gt", "lt", "and", "or", "not"};
static const map<string, Command> commandMap = {
    {"pop", Command::C_POP},
    {"push", Command::C_PUSH},
    {"if-goto", Command::C_IF},
    {"goto", Command::C_GOTO},
    {"function", Command::C_FUNCTION},
    {"call", Command::C_CALL},
    {"return", Command::C_RETURN},
    {"label", Command::C_LABEL},
    {"//", Command::COMMENT}};

bool Parser::getFailedOpen()
{
    return failedOpen;
}

bool Parser::hasMoreCommands()
{
    return vmFile.good();
}

void Parser::removeWhitespace(string &line)
{
    // Trim left whitespace
    size_t charsStart = line.find_first_not_of(" \t");
    if (charsStart != string::npos)
        line.erase(0, charsStart);
    else
    { // entire line is whitespace
        line.clear();
        return;
    }

    size_t slashPos = line.find("//");
    if (slashPos != string::npos && slashPos > 0)
    { // remove inline comments only
        line.erase(slashPos);
    }

    // Trim right whitespace
    size_t charsEnd = line.find_last_not_of(" \t");
    if (charsEnd != string::npos)
        line.erase(charsEnd + 1);
}