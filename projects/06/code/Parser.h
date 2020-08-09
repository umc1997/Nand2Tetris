#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include "Code.h"

using namespace std;

enum Command {A_COMMAND,C_COMMAND,L_COMMAND};
class Parser
{
public:
	Parser(const string& filename);
	~Parser();
//private:
	bool hasMoreCommands();
	void advance();
	Command commandType();
	string symbol();
	string dest();
	string comp();
	string jump();
	void trimLeft(string& line);
	void trimRight(string& line);
	bool isComment(string& line);

	string inputFilename;
	string currentCommand;
	int lineNumber;
	ifstream f;
};
