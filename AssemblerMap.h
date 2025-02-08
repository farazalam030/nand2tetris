#include <iostream>
// #include <bits/stdc++.h>
#include <string>
#include <unordered_map>
#define INVALID_INSTR 0xffff0000;
using namespace std;

class AssemblerMap {
  unordered_map<std::string, uint32_t> AssembleCompMap;
  unordered_map<string, uint32_t> AssembleDestMap;
  unordered_map<string, uint32_t> AssembleJumpMap;
  unordered_map<string, uint32_t> AssembleDefaultSymbolsMap;

public:
  AssemblerMap() {
    AssembleCompMap = {

        {"0", 0b0101010},   {"1", 0b0111111},   {"-1", 0b0111010},
        {"D", 0b0001100},   {"A", 0b0110000},   {"!D", 0b0001101},
        {"!A", 0b0110001},  {"-D", 0b0001111},  {"-A", 0b0110011},
        {"D+1", 0b0011111}, {"A+1", 0b0110111}, {"D-1", 0b0001110},
        {"A-1", 0b0110010}, {"D+A", 0b0000010}, {"D-A", 0b0010011},
        {"A-D", 0b0000111}, {"D&A", 0b0000000}, {"D|A", 0b0010101},
        {"1+D", 0b0011111}, {"A&D", 0b0000000}, {"A|D", 0b0010101},
        {"A+D", 0b0000010}, {"!M", 0b1110001},  {"-M", 0b1110011},
        {"M+1", 0b1110111}, {"M-1", 0b1110010}, {"D+M", 0b1000010},
        {"D-M", 0b1010011}, {"M-D", 0b1000111}, {"D&M", 0b1000000},
        {"D|M", 0b1010101}, {"M+D", 0b1000010}, {"M+1", 0b1110111},
        {"M&D", 0b1000000}, {"M|D", 0b1010101}

    };

    AssembleDestMap = {
        {"NULL", 0b000}, {"M", 0b001},  {"D", 0b010},   {"MD", 0b011},
        {"DM", 0b011},   {"A", 0b100},  {"AM", 0b101},  {"MA", 0b101},
        {"AD", 0b110},   {"DA", 0b110}, {"AMD", 0b111}, {"DAM", 0b111},
        {"MDA", 0b111},
    };

    AssembleJumpMap = {

        {"NOP", 0b000}, {"JGT", 0b001}, {"JEQ", 0b010}, {"JGE", 0b011},
        {"JLT", 0b100}, {"JNE", 0b101}, {"JLE", 0b110}, {"JMP", 0b111},

    };

    AssembleDefaultSymbolsMap = {

        {"SP", 0},   {"LCL", 1},        {"ARG", 2},      {"THIS", 3},
        {"THAT", 4}, {"SCREEN", 16384}, {"KBD ", 24576}, {"R0", 0},
        {"R1", 1},   {"R2", 2},         {"R3", 3},       {"R4", 4},
        {"R5", 5},   {"R6", 6},         {"R7", 7},       {"R8", 8},
        {"R9", 9},   {"R10", 10},       {"R11", 11},     {"R12", 12},
        {"R13", 13}, {"R14", 14},       {"R15", 15},
    };
  }

  uint32_t getCompBinaryCode(string computeInstr);
  uint32_t getDestBinaryCode(string dest);
  uint32_t getJumpBinaryCode(string jumpInstr);
  uint32_t getDefaultBinaryCode(string defaultInstr);

  //   uuint32_t32_t getBinaryCode(string computeInstr);
};
