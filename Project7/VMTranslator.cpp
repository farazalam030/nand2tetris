#include "CodeWriter.h"
#include "Parser.h"

using namespace std;

namespace fs = std::filesystem;

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    cout << "Usage: ./VMTranslator <inputVmFile.vm>" << endl;
    return -1;
  }

  string inputVmFile = argv[1];
  CodeWriter codeWriter(inputVmFile);

  Parser parser(inputVmFile);
  int lineNumber = 0;
  while (parser.hasMoreCommands()) {
    string currentCommand = parser.advance();
    int cmd = parser.getCommandType();
    // cmd = C_ARITHMETIC;
    if (cmd == C_ARITHMETIC) {
      codeWriter.writeArithmetic(parser.argument1());
    } else if (cmd == C_PUSH || cmd == C_POP) {
      codeWriter.writePushPop(parser.getCmd(), parser.getArg1(),
                              parser.getArg2());
    }
    lineNumber++;
  }

  cout << inputVmFile << " Translation completed." << endl;
  codeWriter.close();
  return 0;
}