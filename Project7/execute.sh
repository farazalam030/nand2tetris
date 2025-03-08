#!/bin/bash

./build.sh ./MemoryAccess/BasicTest/BasicTest.vm $1 &> debug.log
./build.sh ./MemoryAccess/StaticTest/StaticTest.vm $1 >> debug.log
./build.sh ./MemoryAccess/PointerTest/PointerTest.vm $1 >> debug.log
./build.sh ./StackArithmetic/StackTest/StackTest.vm $1 >> debug.log
./build.sh ./StackArithmetic/SimpleAdd/SimpleAdd.vm $1 >> debug.log
