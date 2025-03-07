#pragma once

#ifndef SHARED_H
#define SHARED_H
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
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

std::string ltrim(const std::string &s) {
  std::string result = s;
  result.erase(result.begin(),
               std::find_if(result.begin(), result.end(), [](unsigned char ch) {
                 return !std::isspace(ch);
               }));
  return result;
}

// Trim trailing whitespaces
std::string rtrim(const std::string &s) {
  std::string result = s;
  result.erase(std::find_if(result.rbegin(), result.rend(),
                            [](unsigned char ch) { return !std::isspace(ch); })
                   .base(),
               result.end());
  return result;
}

// Trim both leading and trailing whitespaces
std::string trim(const std::string &s) { return ltrim(rtrim(s)); }

#endif // !SHARED_H