#include "CodeWriter.h"

constexpr int TEMP_START = 5;
using namespace std;

CodeWriter::CodeWriter(const string &basefileName)
    : baseFileName(basefileName), labelCounter(0) {

  unordered_map<string, Segment> segmentEnumMap = {
      {"local", Segment::LOCAL},
      {"this", Segment::THIS},
      {"that", Segment::THAT},
      {"argument", Segment::ARG},
      {"constant", Segment::CONST},
      {"static", Segment::STATIC},
      {"pointer", Segment::POINTER},
      {"temp", Segment::TEMP}

  };

  asmFile = ofstream(baseFileName + ".asm");
  if (!asmFile.is_open()) {
    cout << "Error opening file " << baseFileName << ".asm" << endl;
  } else {
    cout << "File " << baseFileName << ".asm" << " opened successfully."
         << endl;
  }
}

void CodeWriter::setInputVmFileName(const string &filename) {
  currentVmFile = filename;
  string str = "\n//*************** " + filename + "started ***************\n";
  asmFile << str;
}

void CodeWriter::writeArithmetic(const string &command) {
  string asmCode = "";
  asmCode += "// " + command + "\n";
  if (command == "add" || command == "sub" || command == "and" ||
      command == "or") {
    asmCode += arithmeticLogicalGenerator(command);
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
      "\n//*************** " + currentVmFile + " ended ***************\n";
  str += "(END)\n@END\n0;JMP\n";
  asmFile << str;
  asmFile.close();
  asmFile.clear();
}

string CodeWriter::getMemorySection(const string &segment) {
  string seg;
  if (segment == "local")
    seg = "LCL";
  else if (segment == "argument")
    seg = "ARG";
  else if (segment == "this")
    seg = "THIS";
  else if (segment == "that")
    seg = "THAT";
  else if (segment == "temp")
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

    string seg = getMemorySection(segment);
#ifdef DEBUG
    cout << "Inside PopGenerator" << endl;
    cout << "segment: " << segment << " idx: " << idx << endl;
    cout << "seg: " << seg << endl;
    cout << "segEnum: " << (segmentEnumMap[segment]) << endl;
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
    string staticVar = currentVmFile + "." + idx;
    // SP--
    asmCode += decrementVariable(SP);
    // filename.idx = *SP
    asmCode += pointer_to_variable(staticVar, SP);
  }
  return asmCode;
}

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
    asmCode += "D=M-D\n"; // D=*A-D
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
  if (command == "neg") {
    asmCode += "M=-M\n"; // *A=-*A
  } else if (command == "not") {
    asmCode += "M=!M\n"; // *A=!*A
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
  code += "@" + idx + "\n";     // D=*A
  code += "D=A\n";              // D=idx
  code += "@" + segment + "\n"; // A=LCL

  switch (segmentEnumMap[segment]) {
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

CodeWriter::~CodeWriter() {
  if (asmFile.is_open()) {
    asmFile.close();
  }
}