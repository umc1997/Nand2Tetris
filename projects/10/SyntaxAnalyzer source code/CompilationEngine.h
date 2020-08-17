#pragma once
#include <string>

using namespace std;


class CompilationEngine {
public:
	CompilationEngine(const string& fileName);
	~CompilationEngine();

	void ComplieClass();
	void ComplieClassVarDec();
	void ComplieSubroutine();
	void ComplieParameterList();
	void ComplieVarDec();
	void ComplieStatements();
	void ComplieDo();
	void ComplieLet();
	void ComplieWhile();
	void ComplieReturn();
	void ComplieIf();
	void ComplieExpression();
	void ComplieTerm();
	void ComplieExpressionList();


private:
};