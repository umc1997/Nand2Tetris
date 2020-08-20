#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>

using namespace std;

enum Token { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST };
enum Keyword { CLASS, METHOD, FUNCTION, CONSTRUCTOR, INT, BOOLEAN, CHAR, VOID, VAR, STATIC, FIELD, LET, DO, IF, ELSE, WHILE, RETURN, TRUE, FALSE, JACK_NULL, THIS };

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

	ifstream f;
	string currentToken;

private:
	bool isOneLineComment(string& line);
	void trimLeft(string& line);
	void trimRight(string& line);
	void processTokens();
	queue<string> tokenItems;
	string currentTokens;
	string inputFileName;
	bool isinProcessing;
	unordered_map<string, Token>   TokenMap;
	unordered_map<string, Keyword> KeywordMap;
};