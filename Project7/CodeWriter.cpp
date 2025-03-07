#include "CodeWriter.h"

constexpr int TEMP_START = 5;
using namespace std;

CodeWriter::CodeWriter(const string &outputAsmFile) : asmFile(outputAsmFile) {
  if (!asmFile.is_open()) {
    cout << "Error opening file " << outputAsmFile << endl;
  } else {
    cout << "File " << outputAsmFile << " opened successfully." << endl;
  }
  labelCounter = 0
}

void CodeWriter::setFileName(const string &filename) {
  currentVmFile = filename;
  string str =
      "\n//*************** " + filename + ".vm started ***************\n";
  asmFile << str;
}

void CodeWriter::writeArithmetic(const string &command) {
  string asmCode = "";
  asmCode += "// " + command + "\n";
  if (command == "add" || command == "sub" || command == "and" ||
      command == "or") {
    asmCode += arithmeticBinaryGenerator(command);
  } else if (command == "neg" || command == "not") {
    asmCode += arithmeticNegNotGenerator(command);
  } else if (command == "eq" || command == "gt" || command == "lt") {
    asmCode += arithmeticComparisonGenerator(command);
  } else {
    cout << "Invalid arithmetic command: " << command << endl;
  }
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
void CodeWriter::close() {
  string str =
      "\n//*************** " + currentVmFile + ".vm ended ***************\n";
  str += "(END)\n@END\n0;JMP\n";
  asmFile << str;
  asmFile.close();
  asmFile.clear();
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

    string seg = segmentMap[segment];
    // push segment idx -> [*SP=*seg+idx, SP++]
    asmCode +=
        ithSegment(addr, segmentEnumMap[segment], idx); // addr = SEgment + idx
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
    string staticVar = currentVmFile + "." + idx;
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

    // addr = segment + idx
    asmCode += ithSegment(addr, segmentEnumMap[segment], idx);
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
    string staticVar = currentVmFile + "." + idx;
    // SP--
    asmCode += decrementVariable(SP);
    // filename.idx = *SP
    asmCode += pointer_to_variable(staticVar, SP);
  }
  return asmCode;
}

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
  code += "D=M\n";               // D=*A
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
string CodeWriter::ithSegment(string destination, Segment segment, string idx) {
  string code = "";
  code += "@" + idx + "\n";                 // D=*A
  code += "D=A\n";                          // D=idx
  code += "@" + segmentMap[segment] + "\n"; // A=LCL

  switch (segment) {
  case Segment::TEMP:
    code += "D=D+A\n"; // D=idx+5
    break;
  default:
    code += "D=D+M\n"; // D=idx+*A
    break;
  }
  code += "@" + destination + "\n";
  code += "M=D\n";
  return code;
}