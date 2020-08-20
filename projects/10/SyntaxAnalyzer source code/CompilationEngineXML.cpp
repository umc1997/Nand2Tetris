#include "CompilationEngineXML.h"

CompilationEngine::CompilationEngine(const string& inputFileName,const string& outputFileName)
:inputFileName(inputFileName), outputFileName(outputFileName), myTokenizer(inputFileName)
{
	outputFile.open(this->outputFileName);
	tapCount = 0;
}
CompilationEngine::~CompilationEngine()
{
	outputFile.close();
}

void CompilationEngine::CompileFile()
{
	if (!myTokenizer.hasMoreTokens()) throw("Error:It is empty file.");
	CompileClass();
	/*while (myTokenizer.hasMoreTokens())
	{
		myTokenizer.advance();
		currentToken = myTokenizer.currentToken;
		switch (myTokenizer.tokenType())
		{
		case(KEYWORD):
			cout << "This is keyword: " << myTokenizer.currentToken << endl;
			break;
		case(SYMBOL):
			cout << "This is symbol: " << myTokenizer.symbol() << endl;
			break;
		case(IDENTIFIER):
			cout << "This is indentifier: " << myTokenizer.identifier() << endl;
			break;
		case(INT_CONST):
			cout << "This is constant int: " << to_string(myTokenizer.intVal()) << endl;
			break;
		case(STRING_CONST):
			cout << "This is constant string: " << myTokenizer.stringVal() << endl;
			break;
		}
	}*/
}
void CompilationEngine::CompileClass()
{
	cout << "<class>" << endl;
	tapCount++;
	//<body>
	myTokenizer.advance();
	CompileKeyword();

	myTokenizer.advance();
	CompileIdentifier();

	myTokenizer.advance();
	CompileSymbol();
	//
	while (!(myTokenizer.currentToken == "}")) {
		myTokenizer.advance();
		if (myTokenizer.keyword() == STATIC || myTokenizer.keyword() == FIELD) //STATIC || FIELD
			CompileClassVarDec();
		if (myTokenizer.keyword() == CONSTRUCTOR || myTokenizer.keyword() == FUNCTION || myTokenizer.keyword() == METHOD) //CONSTRUCTOR || FUNCTION || METHOD
			CompileSubroutine();
	} 
	//
	CompileSymbol();
	//</body>

	tapCount--;
	cout << "</class>" << endl;
}
void CompilationEngine::CompileClassVarDec()
{
	CompileKeyword();
	myTokenizer.advance();
	// type
	CompileType();
	// varName
	do
	{
		if (myTokenizer.currentToken == ",")
			myTokenizer.advance();
		myTokenizer.advance();
		if (myTokenizer.tokenType() == IDENTIFIER)
			CompileIdentifier();
		else; //error
	} while (!(myTokenizer.currentToken == ";"));
	CompileSymbol();
}
void CompilationEngine::CompileSubroutine()
{
	cout << tapAppend(tapCount)<< "<subroutineDec>" << endl;
	tapCount++;
	CompileKeyword();
	// VOID || TYPE
	myTokenizer.advance();
	if (myTokenizer.tokenType() == KEYWORD && myTokenizer.keyword() == VOID)
		CompileKeyword();
	else
		CompileType();
	
	//SubroutineName
	myTokenizer.advance();
	if (myTokenizer.tokenType() == IDENTIFIER)
		CompileIdentifier();
	else;//error
	
	//(parameterlist)

	myTokenizer.advance();
	if (myTokenizer.tokenType() == SYMBOL && myTokenizer.currentToken == "(")
		CompileSymbol();
	//
	cout << tapAppend(tapCount) << "<parameterList>";
	myTokenizer.advance();
	if (myTokenizer.currentToken != ")")
	{
		tapCount++;
		while (myTokenizer.currentToken != ")")
		{
			if (myTokenizer.currentToken == ",")
				myTokenizer.advance();
			CompileType();
			myTokenizer.advance();
			CompileIdentifier();
		}
		tapCount--;
		cout << tapAppend(tapCount)<< "</parameterList>" << endl;
	}
	else
		cout << "</parameterList>" << endl;
	
	CompileSymbol();
	//
	
	// subroutine body
	cout << tapAppend(tapCount)<< "<subroutineBody>" << endl;
	tapCount++;
	myTokenizer.advance();
	if (myTokenizer.tokenType() == SYMBOL)
	{
		if (myTokenizer.symbol() == '{')
		{
			CompileSymbol();
			while (myTokenizer.currentToken != "}")
			{
				myTokenizer.advance();
				//VarDEC
				if (myTokenizer.tokenType() == KEYWORD && myTokenizer.keyword() == VAR)
					CompileVarDec();
				else if(myTokenizer.keyword() == LET|| myTokenizer.keyword() == IF || myTokenizer.keyword() == WHILE|| myTokenizer.keyword() == DO|| myTokenizer.keyword() == RETURN)
				{
					//statements
					cout << tapAppend(tapCount) << "<statements>" << endl;
					CompileStatements();
					cout << tapAppend(tapCount) << "</statements>" << endl;
				}
			}
			
			
			CompileSymbol();
		}
	}
	tapCount--;
	cout << tapAppend(tapCount)<< "</subroutineBody>" << endl;
	tapCount--;
	cout << tapAppend(tapCount)<< "</subroutineDec>" << endl;
}
void CompilationEngine::CompileParameterList()
{}
void CompilationEngine::CompileVarDec()
{
	cout << tapAppend(tapCount) << "<varDec>" << endl;
	tapCount++;
	CompileKeyword();
	myTokenizer.advance();
	CompileType();
	//need to modify
	tapCount--;
	cout << tapAppend(tapCount) << "</varDec>" << endl;
}
void CompilationEngine::CompileStatements()
{
	cout << tapAppend(tapCount) << "<statements>" << endl;
	tapCount++;
	CompileKeyword();

	tapCount--;
	cout << tapAppend(tapCount) << "</statements>" << endl;
}
void CompilationEngine::CompileDo()
{}
void CompilationEngine::CompileLet()
{}
void CompilationEngine::CompileWhile()
{}
void CompilationEngine::CompileReturn()
{}
void CompilationEngine::CompileIf()
{}
void CompilationEngine::CompileExpression()
{}
void CompilationEngine::CompileTerm()
{}
void CompilationEngine::CompileExpressionList()
{}

void CompilationEngine::CompileType()
{
	if (myTokenizer.tokenType() == KEYWORD)
	{
		if (myTokenizer.keyword() == INT)
			CompileKeyword();
		else if (myTokenizer.keyword() == CHAR)
			CompileKeyword();
		else if (myTokenizer.keyword() == BOOLEAN)
			CompileKeyword();
	}
	else if (myTokenizer.tokenType() == IDENTIFIER)
		CompileIdentifier();
	else; // error
}
void CompilationEngine::CompileKeyword(){
	
	if (myTokenizer.tokenType() != KEYWORD) throw ("Error: Syntax Error: Keyword.");
	writeKeyword(myTokenizer.currentToken);
}
void CompilationEngine::CompileIdentifier() {
	if (myTokenizer.tokenType() != IDENTIFIER) throw ("Error: Syntax Error: Identifier.");
	writeIdentifier(myTokenizer.identifier());
}
void CompilationEngine::CompileSymbol() {
	if (myTokenizer.tokenType() != SYMBOL) throw ("Error: Syntax Error: Symbol.");
	writeSymbol(myTokenizer.symbol());
}
void CompilationEngine::CompileIntVal() {
	if (myTokenizer.tokenType() != INT_CONST) throw ("Error: Syntax Error: Integer.");
	writeIntVal(myTokenizer.intVal());
}
void CompilationEngine::CompileStringVal() {
	if (myTokenizer.tokenType() != STRING_CONST) throw ("Error: Syntax Error: String.");
	writeStringVal(myTokenizer.stringVal());
}

void CompilationEngine::writeKeyword(const string& s)
{
	cout << tapAppend(tapCount) << "<keyword>" << s << "</keyword>" << endl;
}
void CompilationEngine::writeIdentifier(const string& s)
{
	cout << tapAppend(tapCount) << "<identifier>" << s << "</identifier>" << endl;
}
void CompilationEngine::writeSymbol(const char c)
{
	string tmp;
	tmp += c;
	cout << tapAppend(tapCount) << "<symbol>" << tmp << "</symbol>" << endl;
}
void CompilationEngine::writeIntVal(const int i)
{
	cout << tapAppend(tapCount) << "<integerConstant>" << to_string(i) << "</integerConstant>" << endl;
}
void CompilationEngine::writeStringVal(const string& s)
{
	cout << tapAppend(tapCount) << "<stringConstant>" << s << "</stringConstant>" << endl;
}
string CompilationEngine::tapAppend(int count)
{
	string t = "";
	for (int i = 0; i < count; i++)
		t.append("\t");
	return t;
}