#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"

using namespace std;


class CompilationEngine {
public:
	CompilationEngine(const string& inputFileName, const string& className);
	~CompilationEngine();

	void advance();
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

	Keyword CompileKeyword();
	string CompileIdentifier();
	string CompileSymbol();
	string CompileIntVal();
	string CompileStringVal();
	string CompileType();
	string CompileSubroutineCall();
	string CompileSubroutineCallwithoutName();

private:
	JackTokenizer myTokenizer;
	SymbolTable mySymbolTable;
	VMWriter myVMWriter;
	string inputFileName;
	string className;
	ofstream outputFile;
	int tapCount;

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