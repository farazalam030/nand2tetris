#!/bin/bash
DEBUG=""
if [ "$2" == "1" ]; then
    DEBUG="-DDEBUG"
fi

g++ ${DEBUG} -std=c++20 -g *.cpp -o VMTranslator.o && ./VMTranslator.o $1
