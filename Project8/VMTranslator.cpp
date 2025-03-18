#include "CodeWriter.h"
#include "Parser.h"

using namespace std;

namespace fs = std::filesystem;

void vmTranslator(Parser &parser, CodeWriter &codeWriter, string &filePath,
                  bool isMultiVM, bool sysInitNeeded);

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    cout << "Usage: ./VMTranslator.o <vmFile_OR_DirectoryContainingsVM>"
         << endl;
    cout << "Example ./VMtranslator.o function_directory " << endl;
    cout << "Note: The directory path should not suffixed with char /" << endl;
    return -1;
  }

  string filePath = argv[1];
  cout << "filePath: " << filePath << endl;
  string sysFilePath = "";
  string inputVMFilePath = "";
  vector<string> vmFiles;
  map<string, string> vmFilesMap;
  bool isMultipleVMs = false;
  bool sysInitNeeded = false;
  if (fs::is_directory(fs::absolute(filePath))) {
    try {

      for (auto &entry : fs::directory_iterator(filePath)) {
        if (entry.path().extension() == ".vm") {

          vmFiles.push_back(entry.path());
          vmFilesMap[entry.path().filename()] = entry.path();
        }
        if (entry.path().filename() == "Sys.vm") {
          sysFilePath = entry.path();
          sysInitNeeded = true;
          inputVMFilePath =
              sysFilePath.substr(0, sysFilePath.find_last_of('/'));
        }
      }
    } catch (const fs::filesystem_error &e) {
      cout << e.what() << endl;
    }
  } else if (fs::is_regular_file(filePath)) {
    inputVMFilePath = filePath;
  }
  cout << "sysFilePath: " << sysFilePath << endl;
  cout << "inputVMFilePath: " << inputVMFilePath << endl;

  if (vmFilesMap.size() > 1)
    isMultipleVMs = true;

  Parser parser(inputVMFilePath);
  CodeWriter codeWriter(inputVMFilePath, isMultipleVMs, sysInitNeeded);

  if (vmFilesMap.size() > 1) {
    for (auto &i : vmFilesMap) {
      inputVMFilePath = i.second;
#ifdef DEBUG
      cout << __FILE__ << ":" << __LINE__ << " ";
      cout << i.first << " " << i.second << endl;
      cout << "inputVMFilePath : " << inputVMFilePath << endl;
#endif // DEBUG

      vmTranslator(parser, codeWriter, inputVMFilePath, true, true);
    }
  } else {
    // vmTranslator(inputVMFilePath);

    cout << "Single file" << inputVMFilePath << endl;
    vmTranslator(parser, codeWriter, inputVMFilePath, false, false);
  }

  codeWriter.close();

  return 0;
}

void vmTranslator(Parser &parser, CodeWriter &codeWriter, string &filePath,
                  bool isMultiVM, bool sysInitNeeded) {
  codeWriter.setisMultipleVMs(isMultiVM);
  codeWriter.setsysInitNeeded(sysInitNeeded);
  codeWriter.setcurrentVmFile(filePath);

  parser.setVmFile(filePath);

  int lineNumber = 0;
  while (parser.hasMoreCommands()) {
    string currentCommand = parser.advance();
    int cmdType = parser.getCommandType();
    if (cmdType == C_ARITHMETIC) {
      codeWriter.writeArithmetic(parser.argument1());
    } else if (cmdType == C_PUSH || cmdType == C_POP) {
      codeWriter.writePushPop(parser.getCmd(), parser.getArg1(),
                              parser.getArg2());
    } else if (cmdType == C_LABEL) {
      codeWriter.writeLabel(parser.getArg1());
    } else if (cmdType == C_GOTO) {
      codeWriter.writeGoto(parser.getArg1());
    } else if (cmdType == C_IF) {
      codeWriter.writeIf(parser.getArg1());
    } else if (cmdType == C_FUNCTION) {
      codeWriter.writeFunction(parser.getArg1(), parser.getArg2());
    } else if (cmdType == C_RETURN) {
      codeWriter.writeReturn();
    } else if (cmdType == C_CALL) {
      codeWriter.writeCall(parser.getArg1(), parser.getArg2());
    }

    lineNumber++;
  }
}