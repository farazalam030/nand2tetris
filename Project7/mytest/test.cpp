#include <bits/stdc++.h>
using namespace std;

// Trim leading whitespaces
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

int main(int argc, char const *argv[]) {
  /* code */
  string a = "I am faraz alam push constant 7";
  ifstream vmFile("/home/faraz/skillup/nandTOtetris/Project7/MemoryAccess/"
                  "BasicTest/BasicTest.vm");
  while (vmFile.peek() != EOF) {
    string line;
    getline(vmFile, line);
    line = trim(line);
    if (line.length() == 0 || line[0] == '/' && line[1] == '/' ||
        line == "\n") {
      continue;
    }
    cout << line << endl;
  }

  //   stringstream tokens(a);
  //   string token = "";
  //   while (tokens >> token) {

  //     if (token.length() == 0) {
  //       continue;
  //     }

  //     cout << token << endl;
  //   }
  return 0;
}
