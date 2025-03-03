#pragma once
#include <string>
#include <fstream>
#include <set>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <filesystem>

#ifndef SHARED_H
#define SHARED_H

enum class Command
{
    NONE,
    COMMENT,
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL
};

#endif // !SHARED_H