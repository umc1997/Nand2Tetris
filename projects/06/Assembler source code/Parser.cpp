#include "Parser.h"

using namespace std;

Parser::Parser(const string& filename) {

	inputFilename = filename;
	lineNumber = 0;
	f.open(filename.c_str(), ifstream::in);
};
Parser::~Parser()
{
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
		lineNumber++;
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
		lineNumber++;
		trimLeft(s);
	} while (isComment(s) || s.empty());

	trimRight(s);
	currentCommand = s;
}
Command Parser::commandType() const
{
	if (currentCommand.find("@") == 0)
	{
		return A_COMMAND;
	}
	else if (currentCommand.find("(") != string::npos)
	{
		return L_COMMAND;
	}
	else
	{
		return C_COMMAND;
	}
}
string Parser::symbol() const {
	string s = currentCommand;
	s.erase(0, 1);
	if (s.find(")") != string::npos)
	{
		s.erase(s.find(")"));
	}
	return s;
}
string Parser::dest() const {
	string s = currentCommand;
	if (s.find("=") != string::npos)
	{
		return s.erase(s.find("="),string::npos);

	}
	else
	{
		return "";
	}

}
string Parser::comp() const {
	string s = currentCommand;

	size_t end = s.find(";");
	if (end != string::npos)
	{
		s.erase(end, string::npos);
	}
	size_t begin = s.find("=");
	if (begin != string::npos)
	{
		s.erase(0, begin + 1);
	}
	return s;
}
string Parser::jump() const {
	string s = currentCommand;
	if (s.find(";") != string::npos)
	{
		s.erase(0, s.find(";") + 1);
		return s;
	}
	else
	{
		return "";
	}
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
bool Parser::isComment(string& line)
{
	return line.find("//") == 0;
}
int Parser::getLineNumber()
{
	return lineNumber;
}