#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include "Code.h"

using namespace std;

enum Command { A_COMMAND, C_COMMAND, L_COMMAND };
class Parser
{
public:
	Parser(const string& filename);
	~Parser();

	bool hasMoreCommands();
	void advance();
	Command commandType() const;
	string symbol() const;
	string dest() const;
	string comp() const;
	string jump() const;
	void trimLeft(string& line);
	void trimRight(string& line);
	bool isComment(string& line);
	int getLineNumber();

	string inputFilename;
	string currentCommand;
	int lineNumber;
	ifstream f;
};
