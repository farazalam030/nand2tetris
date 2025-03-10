#include "CodeWriter.h"

constexpr int TEMP_START = 5;
using namespace std;

CodeWriter::CodeWriter(const string &filename)
    : currentVmFile(filename), labelCounter(0) {

  baseFileName = currentVmFile.substr(currentVmFile.find_last_of('/') + 1);
  baseFileName = baseFileName.substr(0, baseFileName.find_last_of('.') - 0);

  asmfileName =
      currentVmFile.substr(0, currentVmFile.find_last_of('.')) + ".asm";
  asmFile = ofstream(asmfileName);
  if (!asmFile.is_open()) {
    cout << "Error opening file " << asmfileName << endl;
  } else {
    cout << "File " << asmfileName << " opened successfully." << endl;
  }
  putCommentVMFileName(baseFileName);
}
#ifdef DEBUG
void CodeWriter::printDebugInfo() {
  cout << "currentVmFile: " << currentVmFile << endl;
  cout << "baseFileName: " << baseFileName << endl;
  cout << "labelCounter: " << labelCounter << endl;
  cout << "asmFile: " << asmFile.is_open() << endl;
  cout << "asmfileName: " << asmfileName << endl;
}
#endif // DEBUG

void CodeWriter::putCommentVMFileName(const string &filename) {
// currentVmFile = filename;
#ifdef DEBUG
  cout << "Inside putCommentVMFileName" << endl;
  printDebugInfo();
#endif // DEBUG

  string str =
      "\n//*************** " + filename + ".vm started ***************\n";
  asmFile << str;
}

void CodeWriter::writeArithmetic(const string &command) {
  string asmCode = "";
  asmCode += "// " + command + "\n";
#ifdef DEBUG
  cout << "Inside writeArithmetic" << endl;
  cout << "command: " << command << endl;
#endif // DEBUG
  if (command.compare("add") == 0 || command.compare("sub") == 0 ||
      command.compare("and") == 0 || command.compare("or") == 0) {
    asmCode += arithmeticLogicalGenerator(command);
  } else if (command == "neg" || command == "not") {
    asmCode += arithmeticNegNotGenerator(command);
  } else if (command == "eq" || command == "gt" || command == "lt") {
    asmCode += arithmeticComparisonGenerator(command);
  } else {
    cout << "Invalid arithmetic command: " << command << endl;
  }
  asmFile << asmCode;
}

void CodeWriter::writePushPop(const string &command, const string &segment,
                              int index) {
  string asmCode = "";
  if (command == "push") {
    asmCode += pushGenerator(segment, index);
  } else if (command == "pop") {
    asmCode += popGenerator(segment, index);
  } else {
    cout << "Invalid push/pop command: " << command << endl;
  }
  asmFile << asmCode;
}

void CodeWriter::writeLabel(const string &label) {
  string asmCode = labelGenerator(label);
  asmFile << asmCode;
}

void CodeWriter::writeGoto(const string &label) {
  string asmCode = gotoGenerator(label);
  asmFile << asmCode;
}

void CodeWriter::writeIf(const string &label) {
  string asmCode = ifGenerator(label);
  asmFile << asmCode;
}

void CodeWriter::close() {
  string str =
      "\n//*************** " + baseFileName + ".vm ended ***************\n";
  str += "(END)\n@END\n0;JMP\n";
  asmFile << str;
  asmFile.close();
  asmFile.clear();
}

string CodeWriter::getMemorySection(const string &segment) {
  string seg;
  if (segment.compare("local") == 0)
    seg = "LCL";
  else if (segment.compare("argument") == 0)
    seg = "ARG";
  else if (segment.compare("this") == 0)
    seg = "THIS";
  else if (segment.compare("that") == 0)
    seg = "THAT";
  else if (segment.compare("temp") == 0)
    seg = "5";
  return seg;
}

string CodeWriter::pushGenerator(const string &segment, int index) {
  string idx = to_string(index);
  string asmCode = "// push " + segment + " " + to_string(index) + "\n";

  if (segment == "constant") {
    // push constant idx ->  [*SP=idx, SP++]
    asmCode += idx_to_ptr(SP, idx);
    asmCode += incrementVariable(SP);
  }

  else if (segment == "local" || segment == "argument" || segment == "this" ||
           segment == "that" || segment == "temp") {

    string seg = getMemorySection(segment);

    asmCode += ithSegment(addr, seg, idx);              // addr = SEgment + idx
    asmCode += pointer_to_pointer(SP, addr);            // *SP = *addr
    asmCode += incrementVariable(SP);
  }

  else if (segment == "pointer") {
    string th_ptr = "";
    if (index == 0) {
      th_ptr = THIS;
    } else { // if (idx == "1") {
      th_ptr = THAT;
    }
    // push pointer idx -> [*SP=THIS/THAT, SP++]
    asmCode += variable_to_pointer(SP, th_ptr); // *SP = THIS/THAT
    asmCode += incrementVariable(SP);
  }

  // push static idx -> [*SP=filename.idx, SP++]
  else {

    string staticVar = baseFileName + "." + idx;
#ifdef DEBUG
    cout << "Inside PushGenerator" << endl;
    cout << baseFileName << " " << idx << endl;
    cout << "staticVar: " << staticVar << endl;
#endif // DEBUG

    asmCode += variable_to_pointer(SP, staticVar); // SP--
    asmCode += incrementVariable(SP);
  }

  return asmCode;
}

string CodeWriter::popGenerator(const string &segment, int index) {
  string idx = to_string(index);
  string asmCode = "// pop " + segment + " " + to_string(index) + "\n";
  if (segment == "local" || segment == "argument" || segment == "this" ||
      segment == "that" || segment == "temp") {

    string seg = getMemorySection(segment);
#ifdef DEBUG
    cout << "Inside PopGenerator" << endl;
    cout << "segment: " << segment << " idx: " << idx << endl;
    cout << "seg: " << seg << endl;
#endif // DEBUG
    // addr = segment + idx
    asmCode += ithSegment(addr, seg, idx);
    // SP--
    asmCode += decrementVariable(SP);

    // *addr = *SP
    asmCode += pointer_to_pointer(addr, SP);

  } else if (segment == "pointer") {
    string th_ptr = "";
    if (index == 0) {
      th_ptr = THIS;
    } else { // if (idx == "1") {
      th_ptr = THAT;
    }
    // SP--
    asmCode += decrementVariable(SP);
    // THIS/THAT = *SP
    asmCode += pointer_to_variable(th_ptr, SP);
  }

  else {
    string staticVar = baseFileName + "." + idx;
    // SP--
    asmCode += decrementVariable(SP);
    // filename.idx = *SP
    asmCode += pointer_to_variable(staticVar, SP);
  }
  return asmCode;
}

////////////////////////// Helper Functions //////////////////////////
string CodeWriter::arithmeticLogicalGenerator(const string &command) {
  string asmCode = "";
  asmCode += decrementVariable(SP); // SP--
  asmCode += "A=M\n";               // A=*SP
  asmCode += "D=M\n";               // D=*A
  asmCode += decrementVariable(SP); // SP--
  asmCode += "A=M\n";               // A=*SP
  if (command == "add") {
    asmCode += "D=D+M\n"; // D=D+*A
  } else if (command == "sub") {
    asmCode += "D=D-M\n"; // D=*A-D
    asmCode += "D=-D\n";  // D=-D
  } else if (command == "and") {
    asmCode += "D=D&M\n"; // D=D&M
  } else if (command == "or") {
    asmCode += "D=D|M\n"; // D=D|M
  }

  asmCode += "M=D\n";               // *A=D
  asmCode += incrementVariable(SP); // SP++
  return asmCode;
}

string CodeWriter::arithmeticNegNotGenerator(const string &command) {
  string asmCode = "";
  asmCode += decrementVariable(SP); // SP--
  asmCode += "A=M\n";               // A=*SP
  asmCode += "D=M\n";               // D=*A
  if (command == "neg") {
    asmCode += "M=-D\n"; // *A=-*A

  } else if (command == "not") {
    asmCode += "M=!D\n"; // *A=!*A
  }
  asmCode += incrementVariable(SP); // SP++
  return asmCode;
}

string CodeWriter::arithmeticComparisonGenerator(const string &command) {
  string asmCode = "";
  string jmp = "";
  string trueLabel = "TRUE" + to_string(labelCounter);
  string falseLabel = "FALSE" + to_string(labelCounter);
  labelCounter++;
  if (command == "eq")
    jmp = "JEQ";
  else if (command == "lt")
    jmp = "JLT";
  else if (command == "gt")
    jmp = "JGT";

  asmCode += decrementVariable(SP); // SP--
  asmCode += "A=M\n";               // A=*SP
  asmCode += "D=M\n";               // D=*A
  asmCode += decrementVariable(SP); // SP--
  asmCode += "A=M\n";               // A=*SP
  asmCode += "D=M-D\n";             // D=*A-D
  asmCode += "@" + trueLabel + "\n";
  asmCode += "D;" + jmp + "\n"; // D;jmp trueLabel
  asmCode += "D=0\n";           // D=0
  asmCode += "@" + falseLabel + "\n";
  asmCode += "0;JMP\n"; // 0;JMP falseLabel
  asmCode += "(" + trueLabel + ")\n";
  asmCode += "D=-1\n"; // D=-1
  asmCode += "(" + falseLabel + ")\n";
  asmCode += "@SP\n";               // A=SP
  asmCode += "A=M\n";               // A=*SP
  asmCode += "M=D\n";               // *A=D
  asmCode += incrementVariable(SP); // SP++
  return asmCode;
}
string CodeWriter::getBaseFileName() { return baseFileName; }

string CodeWriter::idx_to_ptr(string ptr, string idx) {
  string code = "";
  code += "@" + idx + "\n"; // A=idx
  code += "D=A\n";          // D=idx
  code += "@" + ptr + "\n"; // A=ptr
  code += "A=M\n";          // A=*ptr
  code += "M=D\n";          // *ptr=idx
  return code;
}

string CodeWriter::variable_to_pointer(string ptr, string variable) {
  string code = "";
  // *SP = foo.i
  code += "@" + variable + "\n"; // A=variable
  code += "D=M\n";               // D=M
  code += "@" + ptr + "\n";      // A=ptr
  code += "A=M\n";               // A=*ptr
  code += "M=D\n";               // *ptr=*A
  return code;
}
string CodeWriter::pointer_to_variable(string variable, string ptr) {
  string code = "";
  // foo.i = *SP
  code += "@" + ptr + "\n";      // A=ptr
  code += "A=M\n";               // A=*ptr
  code += "D=M\n";               // D=*A
  code += "@" + variable + "\n"; // A=ptr
  code += "M=D\n";               // M=*ptr
  return code;
}
string CodeWriter::pointer_to_pointer(string ptr1, string ptr2) {
  string code = "";
  // *ptr2 = *ptr1
  code += "@" + ptr2 + "\n"; // A=ptr2
  code += "A=M\n";           // A=*ptr2
  code += "D=M\n";           // D=*A
  code += "@" + ptr1 + "\n"; // A=ptr1
  code += "A=M\n";           // A=*ptr1
  code += "M=D\n";           // *ptr1=*ptr2
  return code;
}
string CodeWriter::incrementVariable(string variable) {
  string code = "";
  // variable++
  code += "@" + variable + "\n"; // A=variable
  code += "M=M+1\n";             // *A=*A+1
  return code;
}
string CodeWriter::decrementVariable(string variable) {
  string code = "";
  // variable--
  code += "@" + variable + "\n"; // A=variable
  code += "M=M-1\n";             // *A=*A-1
  return code;
}
string CodeWriter::ithSegment(string destination, string segment, string idx) {
  string code = "";
#ifdef DEBUG
  cout << "destination: " << destination << " segment: " << segment
       << " idx: " << idx << endl;
#endif                          // DEBUG
  code += "@" + idx + "\n";
  code += "D=A\n";              // D=idx
  code += "@" + segment + "\n"; // A=LCL

  if (segment == "5") {
    code += "D=D+A\n"; // D=idx+5
  } else {
    code += "D=D+M\n"; // D=idx+*A
  }
  code += "@" + destination + "\n";
  code += "M=D\n";
  return code;
}

CodeWriter::~CodeWriter() {
  if (asmFile.is_open()) {
    asmFile.close();
  }
}

string CodeWriter::labelGenerator(const string &label) {
  string code = "// ---- label" + label + " ---  //\n";
  code += "(" + label + ")\n";
  return code;
}
string CodeWriter::gotoGenerator(const string &label) {
  string code = "// ---- goto" + label + " ---  //\n";
  code += "@" + label + "\n";
  code += "0;JMP\n";
  return code;
}
string CodeWriter::ifGenerator(const string &label) {
  string code = "// ---- if-goto" + label + " ---  //\n";
  code += decrementVariable(SP);
  code += "A=M\n";
  code += "D=M\n";
  code += "@" + label + "\n";
  code += "D;JNE\n";
  return code;
}