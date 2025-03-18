This is the NAND to Tetris Project 8

To build this project the following minimum requirements needed.

    Project 8 is built on unix based system so unix based system needed to build
      Note: Avoid windows

There are 2 scripts as follows:

    1. build.sh :  Used to compile and build whole project and execute any test case.
        Usage:  ./build.sh <TestCasePath>  optional [1 for debug log]
        Example ./build.sh FunctionCalls/FibonacciElement 1

    2. Project8/buildAndExecuteAllCases.sh : Used to build and execute all test cases.
        Usage : bash buildAndExecuteAllCases.sh