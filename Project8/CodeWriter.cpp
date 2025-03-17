#include "CodeWriter.h"

constexpr int TEMP_START = 5;
using namespace std;

CodeWriter::CodeWriter(const string &filename, const bool &isMultiVM = false,
                       const bool &SysInitNeeded = false)
{
  labelCounter = 0;
  returnLabelCounter = 0;
  isMultipleVMs = isMultiVM;
  sysInitNeeded = SysInitNeeded;
  currentVmFile = filename;
  if (!isMultipleVMs)
  {
#ifdef DEBUG
    cout << "In " << __FILE__ << " " << __LINE__ << endl;
    cout << " currentVMFile: " << currentVmFile << endl;

#endif // DEBUG
    baseFileName = currentVmFile.substr(currentVmFile.find_last_of('/') + 1);
    baseFileName = baseFileName.substr(0, baseFileName.find_last_of('.') - 0);
    asmfileName =
        currentVmFile.substr(0, currentVmFile.find_last_of('.')) + ".asm";
  }
  else
  {

    baseFileName =
        currentVmFile.substr(currentVmFile.find_last_of('/') + 1, string::npos);
    // baseFileName = baseFileName.substr(0, baseFileName.find_last_of('.') -
    // 0);
    asmfileName = currentVmFile + "/" + baseFileName + ".asm";
#ifdef DEBUG
    cout << "In " << __FILE__ << " " << __LINE__ << endl;
    cout << " currentVMFile: " << currentVmFile << endl;
    cout << " baseFileName: " << baseFileName << endl;

#endif // DEBUG
  }

  asmFile = ofstream(asmfileName);
  if (!asmFile.is_open())
  {
    cout << "Error opening file " << asmfileName << endl;
  }
  else
  {
    cout << "File " << asmfileName << " opened successfully." << endl;
  }
  if (sysInitNeeded)
  {
    string init = generateSysInit();
    asmFile << init;
  }

  putCommentVMFileName(currentVmFile);

#ifdef DEBUG
  printDebugInfo();
#endif // DEBUG
}
#ifdef DEBUG
void CodeWriter::printDebugInfo() {
  cout << __FILE__ << " : " << __LINE__ << endl;
  cout << "PRINT DEBUG INFO\n";
  cout << "currentVmFile: " << currentVmFile << endl;
  cout << "baseFileName: " << baseFileName << endl;
  cout << "labelCounter: " << labelCounter << endl;
  cout << "asmFile: " << asmFile.is_open() << endl;
  cout << "asmfileName: " << asmfileName << endl;
  cout << "isMultipleVMs: " << isMultipleVMs << endl;
}
#endif // DEBUG

void CodeWriter::setisMultipleVMs(bool val) { isMultipleVMs = val; }
bool CodeWriter::getisMultipleVMs() { return isMultipleVMs; }

void CodeWriter::setsysInitNeeded(bool val) { sysInitNeeded = val; }
bool CodeWriter::getsysInitNeeded() { return sysInitNeeded; }

void CodeWriter::putCommentVMFileName(const string &filename)
{
// currentVmFile = filename;
#ifdef DEBUG
  cout << "Inside putCommentVMFileName" << endl;
  printDebugInfo();
#endif // DEBUG

  string str =
      "\n//*************** " + filename + ".vm started ***************\n";
  asmFile << str;
}

void CodeWriter::writeArithmetic(const string &command)
{
  string asmCode = "";
  asmCode += "// " + command + "\n";
#ifdef DEBUG
  cout << "Inside writeArithmetic" << endl;
  cout << "command: " << command << endl;
#endif // DEBUG
  if (command.compare("add") == 0 || command.compare("sub") == 0 ||
      command.compare("and") == 0 || command.compare("or") == 0)
  {
    asmCode += arithmeticLogicalGenerator(command);
  }
  else if (command == "neg" || command == "not")
  {
    asmCode += arithmeticNegNotGenerator(command);
  }
  else if (command == "eq" || command == "gt" || command == "lt")
  {
    asmCode += arithmeticComparisonGenerator(command);
  }
  else
  {
    cout << "Invalid arithmetic command: " << command << endl;
  }
  asmFile << asmCode;
}

void CodeWriter::writePushPop(const string &command, const string &segment,
                              int index)
{
  string asmCode = "";
  if (command == "push")
  {
    asmCode += pushGenerator(segment, index);
  }
  else if (command == "pop")
  {
    asmCode += popGenerator(segment, index);
  }
  else
  {
    cout << "Invalid push/pop command: " << command << endl;
  }
  asmFile << asmCode;
}

void CodeWriter::writeLabel(const string &label)
{
  string tmp = currFunc + ":" + label;
  string asmCode = labelGenerator(tmp);
  asmFile << asmCode;
}

void CodeWriter::writeGoto(const string &label)
{
  string tmp = currFunc + ":" + label;
  string asmCode = gotoGenerator(tmp);
  asmFile << asmCode;
}

void CodeWriter::writeIf(const string &label)
{
  string tmp = currFunc + ":" + label;
  string asmCode = ifGenerator(tmp);
  asmFile << asmCode;
}

void CodeWriter::writeCall(const string &functionName, int numArgs)
{
  string asmCode = callGenerator(functionName, numArgs);
  asmFile << asmCode;
}

void CodeWriter::writeReturn()
{
  string asmCode = returnGenerator();
  asmFile << asmCode;
}

void CodeWriter::writeFunction(const string &functionName, int numLocals)
{
  string asmCode = functionGenerator(functionName, numLocals);
  asmFile << asmCode;
}

void CodeWriter::close()
{
  string str =
      "\n//*************** " + baseFileName + ".vm ended ***************\n";
  if (!isMultipleVMs) {
    str += "(END)\n@END\n0;JMP\n";
  }
  asmFile << str;
  asmFile.close();
  asmFile.clear();
}

string CodeWriter::getMemorySection(const string &segment)
{
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

string CodeWriter::pushGenerator(const string &segment, int index)
{
  string idx = to_string(index);
  string asmCode = "// push " + segment + " " + to_string(index) + "\n";

  if (segment.compare("constant") == 0) {
    // push constant idx ->  [*SP=idx, SP++]
    asmCode += idx_to_ptr(SP, idx);
    asmCode += incrementVariable(SP);
  }

  else if (segment.compare("local") == 0 || segment.compare("argument") == 0 ||
           segment.compare("this") == 0 || segment.compare("that") == 0 ||
           segment.compare("temp") == 0) {

    string seg = getMemorySection(segment);

    asmCode += ithSegment(addr, seg, idx);   // addr = SEgment + idx
    asmCode += pointer_to_pointer(SP, addr); // *SP = *addr
    asmCode += incrementVariable(SP);
  }

  else if (segment == "pointer") {
    string th_ptr = "";
    if (index == 0)
    {
      th_ptr = THIS;
    }
    else
    { // if (idx == "1") {
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

string CodeWriter::popGenerator(const string &segment, int index)
{
  string idx = to_string(index);
  string asmCode = "// pop " + segment + " " + to_string(index) + "\n";
  if (segment == "local" || segment == "argument" || segment == "this" ||
      segment == "that" || segment == "temp")
  {

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
  }
  else if (segment == "pointer")
  {
    string th_ptr = "";
    if (index == 0)
    {
      th_ptr = THIS;
    }
    else
    { // if (idx == "1") {
      th_ptr = THAT;
    }
    // SP--
    asmCode += decrementVariable(SP);
    // THIS/THAT = *SP
    asmCode += pointer_to_variable(th_ptr, SP);
  }

  else
  {
    string staticVar = baseFileName + "." + idx;
    // SP--
    asmCode += decrementVariable(SP);
    // filename.idx = *SP
    asmCode += pointer_to_variable(staticVar, SP);
  }
  return asmCode;
}

////////////////////////// Helper Functions //////////////////////////
string CodeWriter::arithmeticLogicalGenerator(const string &command)
{
  string asmCode = "";
  asmCode += decrementVariable(SP); // SP--
  asmCode += "A = M\n";             // A=*SP
  asmCode += "D = M\n";             // D=*A
  asmCode += decrementVariable(SP); // SP--
  asmCode += "A = M\n";             // A=*SP
  if (command == "add")
  {
    asmCode += "D = D + M\n"; // D = D+*A
  }
  else if (command == "sub")
  {
    asmCode += "D = D-M\n"; // D=*A-D
    asmCode += "D = -D\n";  // D = -D
  }
  else if (command == "and")
  {
    asmCode += "D = D&M\n"; // D = D&M
  }
  else if (command == "or")
  {
    asmCode += "D = D|M\n"; // D = D|M
  }

  asmCode += "M = D\n";             // *A = D
  asmCode += incrementVariable(SP); // SP++
  return asmCode;
}

string CodeWriter::arithmeticNegNotGenerator(const string &command)
{
  string asmCode = "";
  asmCode += decrementVariable(SP); // SP--
  asmCode += "A = M\n";             // A=*SP
  asmCode += "D = M\n";             // D=*A
  if (command == "neg")
  {
    asmCode += "M=-D\n"; // *A=-*A
  }
  else if (command == "not")
  {
    asmCode += "M=!D\n"; // *A=!*A
  }
  asmCode += incrementVariable(SP); // SP++
  return asmCode;
}

string CodeWriter::arithmeticComparisonGenerator(const string &command)
{
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
  asmCode += "A = M\n";             // A=*SP
  asmCode += "D = M\n";             // D=*A
  asmCode += decrementVariable(SP); // SP--
  asmCode += "A = M\n";             // A=*SP
  asmCode += "D = M - D\n";         // D=*A-D
  asmCode += "@" + trueLabel + "\n";
  asmCode += "D;" + jmp + "\n"; // D;jmp trueLabel
  asmCode += "D = 0\n";         // D = 0
  asmCode += "@" + falseLabel + "\n";
  asmCode += "0;JMP\n"; // 0;JMP falseLabel
  asmCode += "(" + trueLabel + ")\n";
  asmCode += "D = -1\n"; // D = -1
  asmCode += "(" + falseLabel + ")\n";
  asmCode += "@SP\n";               // A=SP
  asmCode += "A = M\n";             // A=*SP
  asmCode += "M = D\n";             // *A = D
  asmCode += incrementVariable(SP); // SP++
  return asmCode;
}

string CodeWriter::idx_to_ptr(string ptr, string idx)
{
  string code = "";
  code += "@" + idx + "\n"; // A=idx
  code += "D = A\n";        // D=idx
  code += "@" + ptr + "\n"; // A=ptr
  code += "A = M\n";        // A=*ptr
  code += "M = D\n";        // *ptr=idx
  return code;
}

string CodeWriter::variable_to_pointer(string ptr, string variable)
{
  string code = "";
  // *SP = foo.i
  code += "@" + variable + "\n"; // A=variable
  code += "D = M\n";             // D = M
  code += "@" + ptr + "\n";      // A=ptr
  code += "A = M\n";             // A=*ptr
  code += "M = D\n";             // *ptr=*A
  return code;
}
string CodeWriter::pointer_to_variable(string variable, string ptr)
{
  string code = "";
  // foo.i = *SP
  code += "@" + ptr + "\n";      // A=ptr
  code += "A = M\n";             // A=*ptr
  code += "D = M\n";             // D=*A
  code += "@" + variable + "\n"; // A=ptr
  code += "M = D\n";             // M=*ptr
  return code;
}
string CodeWriter::pointer_to_pointer(string ptr1, string ptr2)
{
  string code = "";
  // *ptr2 = *ptr1
  code += "@" + ptr2 + "\n"; // A=ptr2
  code += "A = M\n";         // A=*ptr2
  code += "D = M\n";         // D=*A
  code += "@" + ptr1 + "\n"; // A=ptr1
  code += "A = M\n";         // A=*ptr1
  code += "M = D\n";         // *ptr1=*ptr2
  return code;
}
string CodeWriter::incrementVariable(string variable)
{
  string code = "";
  // variable++
  code += "@" + variable + "\n"; // A=variable
  code += "M = M + 1\n";         // *A=*A+1
  return code;
}
string CodeWriter::decrementVariable(string variable)
{
  string code = "";
  // variable--
  code += "@" + variable + "\n"; // A=variable
  code += "M = M - 1\n";         // *A=*A-1
  return code;
}
string CodeWriter::ithSegment(string destination, string segment, string idx)
{
  string code = "";
#ifdef DEBUG
  cout << "destination: " << destination << " segment: " << segment
       << " idx: " << idx << endl;
#endif // DEBUG
  code += "@" + idx + "\n";
  code += "D = A\n";            // D=idx
  code += "@" + segment + "\n"; // A=LCL

  if (segment == "5")
  {
    code += "D = D + A\n"; // D=idx+5
  }
  else
  {
    code += "D = D + M\n"; // D=idx+*A
  }
  code += "@" + destination + "\n";
  code += "M = D\n";
  return code;
}

CodeWriter::~CodeWriter()
{
  if (asmFile.is_open())
  {
    asmFile.close();
  }
}

string CodeWriter::labelGenerator(const string &label)
{
  string code = "// ---- label" + label + " ---  //\n";
  code += "(" + label + ")\n";
  return code;
}
string CodeWriter::gotoGenerator(const string &label)
{
  string code = "// ---- goto" + label + " ---  //\n";
  code += "@" + label + "\n";
  code += "0;JMP\n";
  return code;
}
string CodeWriter::ifGenerator(const string &label)
{
  string code = "// ---- if-goto" + label + " ---  //\n";
  code += decrementVariable(SP);
  code += "A = M\n";
  code += "D = M\n";
  code += "@" + label + "\n";
  code += "D;JNE\n";
  return code;
}

string CodeWriter::functionGenerator(const string &functionName,
                                     int numLocals)
{
  string code = "// ---- function " + functionName + " " +
                to_string(numLocals) + "---  //\n";
  currFunc = functionName;
  code += "(" + functionName + ")\n";
  for (int i = 0; i < numLocals; i++)
  {
    code += "@0 \n";         // A=idx
    code += "D = A\n";       // D=idx
    code += "@" + SP + "\n"; // A=ptr
    code += "A = M\n";       // A=*ptr
    code += "M = D\n";       // *ptr=idx
    code += incrementVariable(SP);
  }
  return code;
}

string CodeWriter::callGenerator(const string &functionName, int numArgs)
{
  string code =
      "// ---- call " + functionName + " " + to_string(numArgs) + " ---  //\n";
  string returnAddress = functionName + "$ret." + to_string(returnLabelCounter);
  returnLabelCounter++;
  code += "// push return-address\n";
  code += idx_to_ptr(SP, returnAddress);
  code += incrementVariable(SP);
  code += "// push LCL\n";
  code += variable_to_pointer(SP, LCL);
  code += incrementVariable(SP);
  code += "// push ARG\n";
  code += variable_to_pointer(SP, ARG);
  code += incrementVariable(SP);
  code += "// push THIS\n";
  code += variable_to_pointer(SP, THIS);
  code += incrementVariable(SP);
  code += "// push THAT\n";
  code += variable_to_pointer(SP, THAT);
  code += incrementVariable(SP);
  code += "// ARG = SP - numArgs - 5\n";
  code += "@" + to_string(numArgs) + "\n";
  code += "D = A\n";
  code += "@5\n";
  code += "D = D + A\n"; // n+5
  code += "@" + SP + "\n";
  code += "D = D - M\n"; // n+5-SP
  code += "D = -D\n";    // SP -n -5
  code += "@" + ARG + "\n";
  code += "M = D\n"; // ARG=SP-n-5
  code += "// LCL = SP\n";
  code += "@" + SP + "\n";
  code += "D = M\n";
  code += "@" + LCL + "\n";
  code += "M = D\n";
  code += "// goto " + functionName + "\n";
  code += "@" + functionName + "\n";
  code += "0;JMP\n";
  code += "//(" + returnAddress + ")\n";
  code += "(" + returnAddress + ")\n";
  return code;
}

string CodeWriter::returnGenerator()
{
  string code = "// ---- return ---  //\n";
  string FRAME = "R14"; //  TEMP Variable Frame
  string RET = "R13";

  code += "// Frame = LCL\n";
  code += "@" + LCL + "\n";
  code += "D = M\n";
  code += "@" + FRAME + "\n";
  code += "M = D\n";
  code += "// retAddr = *(FRAME - 5)\n";
  code += "@5\n";
  code += "D = A\n";
  code += "@" + FRAME + "\n";
  code += "A = M - D\n";
  code += "D = M\n";
  code += "@" + RET + "\n";
  code += "M = D\n";
  code += "// *ARG = pop()\n";
  code += "@SP\n";
  code += "AM = M - 1";
  code += "D = M"; // D= M[SP] // pop ()
  code += "@ARG\n";
  code += "A = M\n";
  code += "M = D\n"; // *ARG = pop()
  code += "// SP = ARG + 1\n";
  code += "@" + ARG + "\n";
  code += "D = M + 1\n";
  code += "@" + SP + "\n";
  code += "M = D\n";

  code += "// THAT = *(FRAME - 1)\n";
  code += "@" + FRAME + "\n";
  code += "A = M - 1\n";
  code += "D = M\n";
  code += "@THAT\n ";
  code += "M = D\n";
  code += "// THIS = *(endFrame - 2)\n";
  code += "@2\n";
  code += "D = A\n"; // D= 2
  code += "@" + FRAME + "\n";
  code += "A = M - D\n"; // A = M[FRAME] - 2
  code += "D = M\n";     // D = * (M[FRAME] - 2)
  code += "@THIS\n";
  code += "M = D\n"; // THIS = * (M[FRAME] - 2)
  code += "// ARG = *(FRAME - 3)\n";
  code += "@3\n";
  code += "D = A\n"; // D = 3
  code += "@" + FRAME + "\n";
  code += "A = M - D\n"; // A = M[FRAME] - 3
  code += "D = M\n";     // D = * (M[FRAME] - 3)
  code += "@ARG\n";
  code += "M = D\n"; // THIS = * (M[FRAME] - 2)

  code += "// LCL = *(FRAME - 4)\n";
  code += "@4\n";
  code += "D = A\n"; // D = 4
  code += "@" + FRAME + "\n";
  code += "A = M - D\n"; // A = M[FRAME] - 4
  code += "D = M\n";     // D = * (M[FRAME] - 4)
  code += "@LCL\n";
  code += "M = D\n"; // THIS = * (M[FRAME] - 4)

  code += "//goto RET\n";
  code += "@" + RET + "\n";
  code += "A = M\n";
  return code;
}

string CodeWriter::generateSysInit()
{
  string code = "//______ PROGRAM INIT ________\n";
  code += "@256\n";
  code += "D = A\n";
  code += "@SP\n";
  code += "M = D\n"; // SP = 256

  // code += "@1\n";
  // code += "D = A\n";
  // code += "@LCL\n";
  // code += "M = D\n"; // SP = 256

  // code += "@2\n";
  // code += "D = A\n";
  code += "@ARG\n";
  code += "M = D\n"; // SP = 256

  // code += "@3\n";
  // code += "D = A\n";
  // code += "@THIS\n";
  // code += "M = D\n"; // SP = 256

  // code += "@4\n";
  // code += "D = A\n";
  // code += "@THAT\n";
  // code += "M = D\n"; // SP = 256

  // code += "@bootstrap\n";
  // code += "D = A\n";
  // code += "@SP\n";
  // code += "A = M\n";
  // code += "M = D\n";
  // code += "@SP\n";
  // code += "M = M + 1\n";

  // code += "@LCL\n";
  // code += "D = M\n";
  // code += "@SP\n";
  // code += "A = M\n";
  // code += "M = D\n";
  // code += "@SP\n";
  // code += "M = M + 1\n";

  // code += "@ARG\n";
  // code += "D = M\n";
  // code += "@SP\n";
  // code += "A = M\n";
  // code += "M = D\n";
  // code += "@SP\n";
  // code += "M = M + 1\n";

  // code += "@THIS\n";
  // code += "D = M\n";
  // code += "@SP\n";
  // code += "A = M\n";
  // code += "M = D\n";
  // code += "@SP\n";
  // code += "M = M + 1\n";

  // code += "@THAT\n";
  // code += "D = M\n";
  // code += "@SP\n";
  // code += "A = M\n";
  // code += "M = D\n";
  // code += "@SP\n";
  // code += "M = M + 1\n";

  // code += "@5\nD = A\n@SP\nD = M - D\n@ARG\nM = D\n"; // n=0 and ARG = SP
  // -n-5;

  // code += "@SP\nD = M\n@LCL\nM = D\n"; //  LCL = SP
  // code += "@Sys.init\n0;JMP\n(bootstrap)\n";
  code += callGenerator("Sys.init", 0);

  return code;
}