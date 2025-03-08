#!/bin/bash
g++ -std=c++2a -g *.cpp -o VMTranslator.o && ./VMTranslator.o $1
