// #include "AssemblerMap.h"

#include <iostream>
// #include <bits/stdc++.h>
#include <unordered_map>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    unordered_map<std::string, uint32_t> AssembleCompMap = {

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
        {"M|D", 0b1010101}

    };

    string searchText = "D+M";

    int a = AssembleCompMap[searchText];

    cout << a << endl;

    return 0;
}
