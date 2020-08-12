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
	void writePop(int index,bool isAddress);

private:
	ofstream& outputFile;
	string filename;
	string currentCommand;
	int LABEL_NUMBER;
	const int POINTER_ADDRESS = 3;
	const int TEMP_ADDRESS = 5;
};