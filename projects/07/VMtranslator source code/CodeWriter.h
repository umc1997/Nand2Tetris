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
	CodeWriter(ofstream& outputFile);
	void setFileName(string& filename);
	void writerArithmetic(const string& command);
	void writePushPop(COMMAND command, const string& segment, int index);
	void Close();
	void writePush();
	void writePop(int index);

private:
	ofstream& outputFile;
	string filename;
	string currentCommand;
	stack<uint16_t> Stack;
};