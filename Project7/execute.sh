#!/bin/bash
LOGFILE=""
if [ ! -z "$1" ]; then
    # echo "Usage: ./execute.sh <logfilename>"
    LOGFILE=debug.log
    exit 1
fi


./build.sh ./MemoryAccess/BasicTest/BasicTest.vm $1 &> ${LOGFILE}
./build.sh ./MemoryAccess/StaticTest/StaticTest.vm $1 >> ${LOGFILE}
./build.sh ./MemoryAccess/PointerTest/PointerTest.vm $1 >> ${LOGFILE}
./build.sh ./StackArithmetic/StackTest/StackTest.vm $1 >> ${LOGFILE}
./build.sh ./StackArithmetic/SimpleAdd/SimpleAdd.vm $1 >> ${LOGFILE}
