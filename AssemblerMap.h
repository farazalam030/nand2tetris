#include <iostream>
// #include <bits/stdc++.h>
#include <unordered_map>
#include <string>

using namespace std;

class AssemblerMap{
    public:

    unordered_map<string, uint32_t> AssembleCompMap=
    {

        // a=0
        {"0", 0b0101010},
        {"1", 0b0111111},
        {"-1", 0b0111010},
        {"D", 0b0001100},
        {"A", 0b0110000},
        {"!D", 0b0001101},
        {"!A", 0b0110001},
        {"-D", 0b0001111},
        {"-A", 0b0110011},
        {"D+1", 0b0011111},
        {"A+1", 0b0110111},
        {"D-1", 0b0001110},
        {"A-1", 0b0110010},
        {"D+A", 0b0000010},
        {"D-A", 0b0010011},
        {"A-D", 0b0000111},
        {"D&A", 0b0000000},
        {"D|A", 0b0010101},
        {"1+D", 0b0011111},
        {"A&D", 0b0000000},
        {"A|D", 0b0010101},
        {"A+D", 0b0000010},
        // a=1
        {"!M", 0b1110001},
        {"-M", 0b1110011},
        {"M+1", 0b1110111},
        {"M-1", 0b1110010},
        {"D+M", 0b1000010},
        {"D-M", 0b1010011},
        {"M-D", 0b1000111},
        {"D&M", 0b1000000},
        {"D|M", 0b1010101},
        {"M+D", 0b1000010},
        {"M+1", 0b1110111},
        {"M&D", 0b1000000},
        {"M|D", 0b1010101},

    };



unordered_map<string, uint32_t> AssembleDestMap = {
    {"NULL", 0b000},
    {"M", 0b001},
    {"D", 0b010},
    {"MD", 0b011},
    {"DM", 0b011},
    {"A", 0b100},
    {"AM", 0b101},
    {"MA", 0b101},
    {"AD", 0b110},
    {"DA", 0b110},
    {"AMD", 0b111},
    {"DAM", 0b111},
    {"MDA", 0b111},
};

unordered_map<string, uint32_t> AssembleJumpMap  ={

    {"NOP", 0b000},
    {"JGT", 0b001},
    {"JEQ", 0b010},
    {"JGE", 0b011},
    {"JLT", 0b100},
    {"JNE", 0b101},
    {"JLE", 0b110},
    {"JMP", 0b111},

};

};