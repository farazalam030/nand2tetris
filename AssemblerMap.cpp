#include "AssemblerMap.h"

#include <iostream>
// #include <bits/stdc++.h>
#include <bitset>
// #include <string>
// #include <unordered_map>

using namespace std;

int AssemblerMap::getCompBinaryCode(string computeInstr) {
  if (AssembleCompMap.find(computeInstr) != AssembleCompMap.end()) {
    return AssembleCompMap[computeInstr];
  } else {
    cout << "No Instruction like " << computeInstr << endl;
    exit(-1);
  }
}

int AssemblerMap::getDestBinaryCode(string dest) {
  if (AssembleDestMap.find(dest) != AssembleDestMap.end()) {
    return AssembleDestMap[dest];
  } else {
    cout << "No Destination like " << dest << endl;
    exit(-1);
  }
}

int AssemblerMap::getJumpBinaryCode(string jumpInstr) {
  if (AssembleJumpMap.find(jumpInstr) != AssembleJumpMap.end()) {
    return AssembleJumpMap[jumpInstr];
  } else {
    cout << "No Jump Instr supported like " << jumpInstr << endl;
    exit(-1);
  }
}


int main(int argc, char const *argv[]) {
  AssemblerMap asMap;

  string searchText = "M|D";

  int a = asMap.getCompBinaryCode(searchText);

  cout << ((bitset<16>(a << 6 | 0x7 << 13))) << endl;

  return 0;
}
