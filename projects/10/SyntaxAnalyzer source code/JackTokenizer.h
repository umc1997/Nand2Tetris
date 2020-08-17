#pragma once
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

enum Token { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST };
enum Keyword { KCLASS, METHOD, FUNCTION, CONSTRUCTOR, INT, BOOLEAN, CHAR, VOID, VAR, STATIC, FIELD, LET, DO, IF, ELSE, WHILE, RETURN, TRUE, FALSE, K_NULL, THIS };

class JackTokenizer {
public:
	JackTokenizer(const string& fileName);
	~JackTokenizer();

	bool hasMoreTokens();
	void advance();
	Token tokenType();
	Keyword keyword();
	char symbol();
	string identifier();
	int intVal();
	string stringVal();
	
private:

};