#include <bits/stdc++.h>
using namespace std;

namespace fs = std::filesystem;

void printVector(vector<string> &v) {
  for (auto &i : v) {
    cout << i << endl;
  }
}

int main(int argc, char const *argv[]) {

  cout << "Hello World" << endl;

  string filePath = argv[1];
  cout << "filePath: " << filePath << endl;
  string sysFilePath = "";
  string inputVMFilePath = "";
  vector<string> vmFiles;
  map<string, string> vmFilesMap;

  if (fs::is_directory(fs::absolute(filePath))) {
    try {

      for (auto &entry : fs::directory_iterator(filePath)) {
        if (entry.path().extension() == ".vm") {

          vmFiles.push_back(entry.path());
          vmFilesMap[entry.path().filename()] = entry.path();
        }
        if (entry.path().filename() == "Sys.vm") {
          sysFilePath = entry.path();
        }
      }
    } catch (const fs::filesystem_error &e) {
      cout << e.what() << endl;
    }
  } else if (fs::is_regular_file(filePath)) {
    inputVMFilePath = filePath;
  }
  cout << "sysFilePath: " << sysFilePath << endl;
  cout << "inputVMFilePath: " << inputVMFilePath << endl;

  string outASMFilePath =
      sysFilePath.substr(sysFilePath.find_first_of('/') + 1,
                         sysFilePath.find_last_of("/") -
                             sysFilePath.find_first_of('/') - 1) +
      ".asm";
  cout << "outASMFilePath: " << outASMFilePath << endl;
  printVector(vmFiles);

  for (auto &i : vmFilesMap) {
    cout << i.first << " " << i.second << endl;
  }

  cout << vmFilesMap["Sys.vm"] << endl;
  return 0;
}
