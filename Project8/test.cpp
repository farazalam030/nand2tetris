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

  string filePath = argc > 1 ? fs::absolute(argv[1]) : "";
  cout << "filePath: " << filePath << endl;
  string sysFilePath = "";
  string inputVMFilePath = "";
  vector<string> vmFiles;
  if (fs::is_directory(filePath)) {
    try {

      for (auto &entry : fs::directory_iterator(filePath)) {
        if (entry.path().extension() == ".vm") {

          vmFiles.push_back(entry.path());
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

  printVector(vmFiles);

  return 0;
}
