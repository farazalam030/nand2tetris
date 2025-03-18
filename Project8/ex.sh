#!/bin/bash

git clean -fdx
./build.sh FunctionCalls/FibonacciElement 1 &> out.log
# ./build.sh FunctionCalls/StaticsTest 1 &> out.log
rm -rf /mnt/c/Users/Faraz/Desktop/FunctionCalls
cp -ra FunctionCalls/ /mnt/c/Users/Faraz/Desktop/ -v

