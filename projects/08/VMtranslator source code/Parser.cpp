#include "Parser.h"

Parser::Parser(const string& fileName)
:commandMap({
		{"add" ,		C_ARITHMETIC},
		{"sub",			C_ARITHMETIC},
		{"neg",			C_ARITHMETIC},
		{"eq",			C_ARITHMETIC},
		{"gt",			C_ARITHMETIC},
		{"lt",			C_ARITHMETIC},
		{"and",			C_ARITHMETIC},
		{"or",			C_ARITHMETIC},
		{"not",			C_ARITHMETIC},
		{"push",		C_PUSH},
		{"pop",			C_POP},
		{"label",		C_LABEL},
		{"goto",		C_GOTO},
		{"if",			C_IF},
		{"function",	C_FUNCTION},
		{"return",		C_RETURN},
		{"call",		C_CALL}
		})
{
	inputFileName = fileName;
	f.open(inputFileName.c_str(), ifstream::in);
	
}
Parser::~Parser() {
	f.close();
}

bool Parser::hasMoreCommands() {
	if (f.eof())
	{
		return false;
	}
	bool hasMoreCommands = false;
	string s;
	istream::streampos pos = f.tellg();
	f.clear();
	do
	{
		getline(f, s);
		trimLeft(s);
		if (!isComment(s) && !s.empty())
		{
			hasMoreCommands = true;
			break;
		}
	} while (!f.eof());

	f.seekg(pos);
	return hasMoreCommands;
}
void Parser::advance() {
	string s;
	do {
		getline(f, s);
		trimLeft(s);
	} while (isComment(s) || s.empty());
	trimRight(s);
	split(s);
	currentCommand = s;
}
COMMAND Parser::commandType() const{
	
	return commandMap.at(firstArg);
}
string Parser::arg1() const{
	if (commandType() == C_ARITHMETIC)
		return firstArg;
	else
		return secondArg;
}
int Parser::arg2() const {
	return thirdArg;
}
bool Parser::isComment(string& line)
{
	return line.find("//") == 0;
}
void Parser::trimLeft(string& line)
{
	const string blank = " \t\n\f\v\r";
	line.erase(0, line.find_first_not_of(blank));
}
void Parser::trimRight(string& line)
{
	const string blank = " \t\n\f\v\r";
	int commentPos = line.find("//");
	if (commentPos != 0 && commentPos != string::npos)
	{
		line.erase(line.find_first_of("//"), string::npos);
	}
	line.erase(line.find_last_not_of(blank) + 1, string::npos);
}
void Parser::split(string& line) {
	istringstream split(line);
	istream_iterator<string> begin(split), end;
	vector<string> splitItems(begin, end);

	firstArg = splitItems[0];
	secondArg = "";
	thirdArg = NULL;

	if (splitItems.size() >= 2)
	{
		secondArg = splitItems[1];
	}
	if (splitItems.size() == 3)
	{
		thirdArg = stoi(splitItems[2]);
	}
}