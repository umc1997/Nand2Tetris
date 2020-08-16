#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>  
#include <iterator> 
#include <vector> 

using namespace std;

enum COMMAND { C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL };

class Parser
{
public:
	Parser(const string& fileName);
	~Parser();

	bool hasMoreCommands();
	void advance();
	COMMAND commandType() const;
	string arg1() const;
	int arg2() const;

	void split(string& line);
	void trimLeft(string& line);
	void trimRight(string& line);
	bool isComment(string& line);

	ifstream f;
	string inputFileName;
	string currentCommand;

private:
	unordered_map<string, COMMAND> commandMap;
	string firstArg;
	string secondArg;
	int thirdArg;
};