#pragma once
#include "CommonShared.h"
using namespace std;

const string SP = "SP";
const string LCL = "LCL";
const string ARG = "ARG";
const string THIS = "THIS";
const string THAT = "THAT";
const string TEMP = "5";
const string addr = "addr";

class CodeWriter {

private:
  ofstream asmFile;
  string asmfileName;
  string currentVmFile;
  string baseFileName;
  int labelCounter;
  int returnLabelCounter;
  string currFunc;

public:
  bool isMultipleVMs;
  CodeWriter(const string &filename);
  void setisMultipleVMs(bool val);
  bool getisMultipleVMs();
  void putCommentVMFileName(const string &filename);
  void writeArithmetic(const string &command);
  void writePushPop(const string &command, const string &segment, int index);
  void writeLabel(const string &label);
  void writeGoto(const string &label);
  void writeIf(const string &label);
  void writeCall(const string &functionName, int numArgs);
  void writeReturn();
  void writeFunction(const string &functionName, int numLocals);
  void close(); // close output file

  ~CodeWriter();
#ifdef DEBUG
  void printDebugInfo();
#endif // DEBUG
  string pushGenerator(const string &segment, int index);
  string popGenerator(const string &segment, int index);
  string labelGenerator(const string &label);
  string gotoGenerator(const string &label);
  string ifGenerator(const string &label);
  string callGenerator(const string &functionName, int numArgs);
  string returnGenerator();
  string functionGenerator(const string &functionName, int numLocals);

  string arithmeticLogicalGenerator(const string &command);
  string arithmeticNegNotGenerator(const string &command);
  string arithmeticComparisonGenerator(const string &command);
  string getBaseFileName();

  // HELPER Functions
  string idx_to_ptr(string ptr, string idx);               // *SP=idx
  string variable_to_pointer(string ptr, string variable); // *SP=variable
  string pointer_to_variable(string variable, string ptr); // variable=*SP
  string pointer_to_pointer(string ptr1, string ptr2);     // *ptr2=*ptr1
  string incrementVariable(string variable);               // variable++
  string decrementVariable(string variable);               // variable--
  string ithSegment(string destination, string segment,
                    string idx); // segment+i
  string getMemorySection(const string &segment);
};