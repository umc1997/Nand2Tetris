#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include "Parser.h"

using namespace std;


class CodeWriter
{
public:
	CodeWriter(string& outputFileName);
	void setFileName(string& filename);
	void writerArithmetic(const string& command);
	void writePushPop(COMMAND command, const string& segment, int index);
	void writetInit();
	void writeLabel(const string& label);
	void writeGoto(const string& label);
	void writeIf(const string& label);
	void writeCall(const string& functionName, int numArgs);
	void writeReturn();
	void writeFunction(const string& functionName, int numLocals);
	void Close();

	void writePush();
	void writePop(int index,bool needAccessRAM);

private:
	string outputFileName;
	ofstream outputFile;
	string currentFileName;
	string currentCommand;
	int LABEL_NUMBER;
	const int POINTER_ADDRESS = 3;
	const int TEMP_ADDRESS = 5;
};