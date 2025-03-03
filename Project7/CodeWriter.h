#pragma once
#include "CommonShared.h"
using namespace std;
enum class Segment
{
    CONST = 0,
    ARG,
    LOCAL,
    STATIC,
    THIS,
    THAT,
    POINTER,
    TEMP,
    NONE
};

class CodeWriter
{

private:
    const string outputFilename;
    string inputFilename;
    string funcPrefix;
    string currFunction;
    ofstream outputFile;
    unordered_map<string, int> functionCallCount;
    static const set<string> arithCommands;
    static const unordered_map<string, string> segmentMap;
    int arithCount = 0;
    string getSegmentReference(const string &segment, int index);
    static const unordered_map<string, Segment> segmentEnumMap;

public:
    CodeWriter(const string &filename) : outputFilename(filename)
    {
        outputFile.open(outputFilename);
        if (outputFile.is_open())
        {
            cout << "Writing output to " << filename << endl;
        }
        else
        {
            cout << "Error occurred opening file for output." << endl;
        }
    }

    void writeArithmetic(const string &command);
    void writePush(const string &segment, int index, bool beforePop = false);
    void writePop(const string &segment, int index, bool afterPush = false);

    void setFilename(const string &filename);
    // void writeInit();
    // void writeLabel(const string &label);
    // void writeGoto(const string &label);
    // void writeIf(const string &label);
    // void writeFunction(const string &functionName, int numVars);
    // void writeCall(const string &functionName, int numArgs);
    // void writeReturn();
    // void writeComment(const string &comment);

    // void writeReturnBootstrap();
    // void writeCallBootstrap();

    void close(); // close output file

    ~CodeWriter();
};