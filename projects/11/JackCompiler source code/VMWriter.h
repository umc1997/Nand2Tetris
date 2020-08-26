#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "SymbolTable.h"

using namespace std;

class VMWriter
{
public:
	VMWriter(const string& inputFileName);
	void writePush(const string& segment, int Index);
	void writePush(Range r, int Index);
	void writePop(const string& segment, int Index);
	void writePop(Range r, int Index);
	void writerArithmetic(const string& command);
	void writeLabel(const string& label);
	void writeGoto(const string& label);
	void writeIf(const string& label);
	void writeCall(const string& name, int nArgs);
	void writeFunction(const string& name, int nLocals);
	void writeReturn();
	void close();

private:
	string outputFileName;
	ofstream outputFile;
	string inputFileName;
};