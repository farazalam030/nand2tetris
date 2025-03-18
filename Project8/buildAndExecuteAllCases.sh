#!/bin/bash

git clean -fdx
./build.sh FunctionCalls/FibonacciElement 1 &> FibonacciElement.log
./build.sh FunctionCalls/StaticsTest 1 &> StaticsTest.log
./build.sh FunctionCalls/SimpleFunction/SimpleFunction.vm 1 &> SimpleFunction.vm.log
./build.sh ProgramFlow/BasicLoop/BasicLoop.vm 1 &> ProgramFlowBasicLoop.log
./build.sh ProgramFlow/FibonacciSeries/FibonacciSeries.vm 1 &> ProgramFlowFibonacciSeries.vm.log

