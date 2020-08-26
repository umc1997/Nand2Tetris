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
	int CompileExpressionList();
	void CompileKeywordConstant();

	Keyword CompileKeyword();
	string CompileIdentifier();
	string CompileSymbol();
	string CompileIntVal();
	string CompileStringVal();
	string CompileType();
	void CompileSubroutineCall();
	void CompileSubroutineCallwithoutName(const string& name);

private:
	JackTokenizer myTokenizer;
	SymbolTable mySymbolTable;
	VMWriter myVMWriter;
	string inputFileName;
	string className;
	ofstream outputFile;
	int IF_LABEL_NUMBER;
	int WHILE_LABEL_NUMBER;

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