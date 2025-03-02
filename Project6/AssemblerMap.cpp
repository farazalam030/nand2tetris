#include "AssemblerMap.h"
#include "flags.h"

using namespace std;

int AssemblerMap::getCompBinaryCode(string computeInstr) {
  // cout << "computeInstr  = " << computeInstr << endl;
  if (AssembleCompMap.find(computeInstr) != AssembleCompMap.end()) {
    return AssembleCompMap[computeInstr];
  } else {
    return ERROR;
  }
}

int AssemblerMap::getDestBinaryCode(string dest) {
  // cout << "dest = " << dest << endl;
  // if (dest.empty())
  //   return 0;
  if (AssembleDestMap.find(dest) != AssembleDestMap.end()) {
    return AssembleDestMap[dest];
  } else {
    return ERROR;
  }
}

int AssemblerMap::getJumpBinaryCode(string jumpInstr) {
  // cout << "JMP Instr = " << jumpInstr << endl;
  // if (jumpInstr.empty())
  //   return 0;
  if (AssembleJumpMap.find(jumpInstr) != AssembleJumpMap.end()) {
    return AssembleJumpMap[jumpInstr];
  } else {
    return ERROR;
  }
}
