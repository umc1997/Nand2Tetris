#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "JackTokenizer.h"

using namespace std;


class CompilationEngine {
public:
	CompilationEngine(const string& inputFileName, const string& outputFileName);
	~CompilationEngine();

	void CompileFile();
	void CompileClass();
	void CompileClassVarDec();
	void CompileSubroutine();
	void CompileParameterList();
	void CompileVarDec();
	void CompileStatements();
	void CompileDo();
	void CompileLet();
	void CompileWhile();
	void CompileReturn();
	void CompileIf();
	void CompileExpression();
	void CompileTerm();
	void CompileExpressionList();

	void CompileKeyword();
	void CompileIdentifier();
	void CompileSymbol();
	void CompileIntVal();
	void CompileStringVal();
	void CompileType();
	
private:
	JackTokenizer myTokenizer;
	string inputFileName;
	string outputFileName;
	ofstream outputFile;
	int tapCount;
	
	string tapAppend(int count);
	void writeKeyword(const string& s);
	void writeIdentifier(const string& s);
	void writeSymbol(const char c);
	void writeIntVal(const int i);
	void writeStringVal(const string& s);
};