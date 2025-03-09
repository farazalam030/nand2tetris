#!/bin/bash
NTTLOGFILE="/dev/null"
if ! [[ -z "$1" ]]; then
    NTTLOGFILE=debug.log
fi

./build.sh ./MemoryAccess/BasicTest/BasicTest.vm $1 &> ${NTTLOGFILE}
./build.sh ./MemoryAccess/StaticTest/StaticTest.vm $1 >> ${NTTLOGFILE}
./build.sh ./MemoryAccess/PointerTest/PointerTest.vm $1 >> ${NTTLOGFILE}
./build.sh ./StackArithmetic/StackTest/StackTest.vm $1 >> ${NTTLOGFILE}
./build.sh ./StackArithmetic/SimpleAdd/SimpleAdd.vm $1 >> ${NTTLOGFILE}
