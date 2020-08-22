#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "JackTokenizer.h"

using namespace std;


class CompilationEngineXML {
public:
	CompilationEngineXML(const string& inputFileName, const string& outputFileName);
	~CompilationEngineXML();

	void CompileFile();
	void CompileFileTokens();
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
	void CompileSubroutineCall();
	void CompileSubroutineCallwithoutName();

private:
	JackTokenizer myTokenizer;
	string inputFileName;
	string outputFileName;
	ofstream outputFile;
	int tapCount;

	void writeTagStart(const string& tag);
	void writeTagEnd(const string& tag);
	string tapAppend();
	void writeKeyword(const string& s);
	void writeIdentifier(const string& s);
	void writeSymbol(const char c);
	void writeIntVal(const int i);
	void writeStringVal(const string& s);

	bool isTerm();
	bool isExpression();
	bool isKeywordConstant();
	bool isUnaryOperator();
	bool isOperator();
	bool isSubroutineCall();
	bool isParentheseStart();
	bool isParentheseEnd();
	bool isBraceStart();
	bool isBraceEnd();
	bool isBracketStart();
	bool isBracketEnd();
	bool isComma();
	bool isDot();
	bool isSemiCollon();
	bool isStatement();
	bool isType();
};