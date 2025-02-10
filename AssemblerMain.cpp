#include "AssemblerMap.h"
#include "Parser.h"
#include "SymbolTable.h"
#include <bitset>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[]) {
  string inputAsmFile, outputHackFile;
  uint64_t lineNumberROM, newAddress;
  uint64_t lineNumberSource;
  ofstream fout;
  if (argc < 2 || argc > 3) {
    cout << "Usage: " << argv[0]
         << " <inputfilename.asm> <(optional) outputfilename.hack>" << endl;
    exit(1);
  } else {
    inputAsmFile = argv[1];
    if (argc != 3)
      outputHackFile =
          inputAsmFile.substr(0, inputAsmFile.length() - 4) + ".hack";
    else {
      outputHackFile = argv[3];
    }
  }

  fout.open(outputHackFile);

  if (fout.fail()) {
    cout << "Unable to create output hack file" << endl;
    exit(ERROR);
  }

  Parser symbolSource(inputAsmFile);

  SymbolTable symbolTable;
  lineNumberROM = 0;
  lineNumberSource = 0;

  while (1) {
    symbolSource.parseNext(lineNumberSource);
    if (!symbolSource.hasFurtherCommands()) {
      break;
    }

    if (symbolSource.commandType(lineNumberSource) == 'A' ||
        symbolSource.commandType(lineNumberSource) == 'C') {
      lineNumberROM++;
    }
    if (symbolSource.commandType(lineNumberSource) == 'L' &&
        !symbolTable.isPresent(symbolSource.getSymbolMnemonic())) {
      symbolTable.addSymbolsToSymbolTable(symbolSource.getSymbolMnemonic(),
                                          lineNumberROM);
    }
  }

  Parser assemblySrc(inputAsmFile);
  AssemblerMap asmMap;
  lineNumberSource = 0;
  newAddress = 16;
  uint16_t instructionReg;
  while (100) {
    instructionReg = 0;
    assemblySrc.parseNext(lineNumberSource);
    if (!assemblySrc.hasFurtherCommands()) {
      break;
    }

    if (assemblySrc.commandType(lineNumberSource) == 'A') {
      instructionReg = instructionReg & 0x0;
      if (assemblySrc.getSymbolMnemonic().find_first_not_of("0123456789") ==
          string::npos) {
        instructionReg =
            instructionReg |
            (bitset<15>(stoull(assemblySrc.getSymbolMnemonic(), nullptr)));
        fout << bitset<16>(instructionReg).to_string();
      } else {
        if (!symbolTable.isPresent(assemblySrc.getSymbolMnemonic())) {
          symbolTable.addSymbolsToSymbolTable(assemblySrc.getSymbolMnemonic(),
                                              newAddress++);
        }

        int addr =
            symbolTable.getAddressOfSymbol(assemblySrc.getSymbolMnemonic());
        if (addr != NOTPRESENT) {
          instructionReg = instructionReg | (addr);
          fout << bitset<16>(instructionReg).to_string();
        } else {
          cout << "Error in line " << lineNumberSource << endl;
          exit(ERROR);
        }
      }
      fout << endl;
    }

    else if (assemblySrc.commandType(lineNumberSource) == 'C') {
      instructionReg = instructionReg | 0xE000;
      int tmp = asmMap.getCompBinaryCode(assemblySrc.getcompMnemonic());
      ;
      if (tmp != INVALID)
        instructionReg = instructionReg | (tmp << COMPSHIFT);
      else {
        cout << "Error at line" << lineNumberSource << endl;
        exit(ERROR);
      }
      tmp = asmMap.getDestBinaryCode(assemblySrc.getDestMnemonic());

      if (tmp != INVALID)
        instructionReg = instructionReg | (tmp << DEST_SHIFT);
      else {
        cout << "Error at line" << lineNumberSource << endl;
        exit(ERROR);
      }
      tmp = asmMap.getJumpBinaryCode(assemblySrc.getJumpMnemonic());
      if (tmp != INVALID)
        instructionReg = instructionReg | (tmp << JMP_SHIFT);
      else {
        cout << "Error at line" << lineNumberSource << endl;
        exit(ERROR);
      }
      fout << bitset<16>(instructionReg).to_string();
      fout << endl;
    }
  }

  fout.close();
  return 0;
}
