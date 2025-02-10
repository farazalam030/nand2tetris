#include "AssemblerMap.h"
#include "flags.h"
#include <bitset>

using namespace std;

int AssemblerMap::getCompBinaryCode(string computeInstr) {
  if (AssembleCompMap.find(computeInstr) != AssembleCompMap.end()) {
    return AssembleCompMap[computeInstr];
  } else {
    return INVALID;
  }
}

int AssemblerMap::getDestBinaryCode(string dest) {
  if (AssembleDestMap.find(dest) != AssembleDestMap.end()) {
    return AssembleDestMap[dest];
  } else {
    return INVALID;
  }
}

int AssemblerMap::getJumpBinaryCode(string jumpInstr) {
  if (AssembleJumpMap.find(jumpInstr) != AssembleJumpMap.end()) {
    return AssembleJumpMap[jumpInstr];
  } else {
    return INVALID;
  }
}
