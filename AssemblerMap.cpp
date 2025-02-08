#include "AssemblerMap.h"

#include <iostream>
// #include <bits/stdc++.h>
#include <bitset>
// #include <string>
// #include <unordered_map>

using namespace std;

uint32_t AssemblerMap::getCompBinaryCode(string computeInstr) {
  if (AssembleCompMap.find(computeInstr) != AssembleCompMap.end()) {
    return AssembleCompMap[computeInstr];
  } else {
    return INVALID_INSTR;
  }
}

uint32_t AssemblerMap::getDestBinaryCode(string dest) {
  if (AssembleDestMap.find(dest) != AssembleDestMap.end()) {
    return AssembleDestMap[dest];
  } else {
    return INVALID_INSTR;
  }
}

uint32_t AssemblerMap::getJumpBinaryCode(string jumpInstr) {
  if (AssembleJumpMap.find(jumpInstr) != AssembleJumpMap.end()) {
    return AssembleJumpMap[jumpInstr];
  } else {
    return INVALID_INSTR;
  }
}

uint32_t AssemblerMap::getDefaultBinaryCode(string defaultInstr) {
  if (AssembleDefaultSymbolsMap.find(defaultInstr) !=
      AssembleDefaultSymbolsMap.end()) {
    return AssembleDefaultSymbolsMap[defaultInstr];
  } else {
    return INVALID_INSTR;
  }
}

int main(int argc, char const *argv[]) {
  AssemblerMap asMap;

  string searchText = "M|D";

  uint32_t a = asMap.getCompBinaryCode(searchText);

  cout << ((bitset<16>(a << 6 | 0x7 << 13))) << endl;

  a = asMap.getDefaultBinaryCode("R12");

  cout << ((bitset<16>(a))) << endl;

  return 0;
}
