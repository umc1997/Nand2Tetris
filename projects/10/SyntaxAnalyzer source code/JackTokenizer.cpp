#include "JackTokenizer.h"

JackTokenizer::JackTokenizer(const string& fileName)
	:TokenMap({
		{"class",			KEYWORD},
		{"constructor",		KEYWORD},
		{"function",		KEYWORD},
		{"method",			KEYWORD},
		{"field",			KEYWORD},
		{"static",			KEYWORD},
		{"var",				KEYWORD},
		{"int",				KEYWORD},
		{"char",			KEYWORD},
		{"boolean",			KEYWORD},
		{"void",			KEYWORD},
		{"true",			KEYWORD},
		{"false",			KEYWORD},
		{"null",			KEYWORD},
		{"this",			KEYWORD},
		{"let",				KEYWORD},
		{"do",				KEYWORD},
		{"if",				KEYWORD},
		{"else",			KEYWORD},
		{"while",			KEYWORD},
		{"return",			KEYWORD},
		{"{",				SYMBOL},
		{"}",				SYMBOL},
		{"(",				SYMBOL},
		{")",				SYMBOL},
		{"[",				SYMBOL},
		{"]",				SYMBOL},
		{".",				SYMBOL},
		{",",				SYMBOL},
		{";",				SYMBOL},
		{"+",				SYMBOL},
		{"-",				SYMBOL},
		{"*",				SYMBOL},
		{"\\",				SYMBOL},
		{"&",				SYMBOL},
		{"|",				SYMBOL},
		{"<",				SYMBOL},
		{">",				SYMBOL},
		{"=",				SYMBOL},
		{"~",				SYMBOL}
	})
	,KeywordMap({
		{"class",			CLASS},
		{"constructor",		METHOD},
		{"funtion",			FUNCTION},
		{"method",			METHOD},
		{"field",			FIELD},
		{"static",			STATIC},
		{"var",				VAR},
		{"int",				INT},
		{"char",			CHAR},
		{"boolean",			BOOLEAN},
		{"void",			VOID},
		{"true",			TRUE},
		{"false",			FALSE},
		{"null",			JACK_NULL},
		{"this",			THIS},
		{"let",				LET},
		{"do",				DO},
		{"if",				IF},
		{"else",			ELSE},
		{"while",			WHILE},
		{"return",			RETURN} 
	})
{
	inputFileName = fileName;
	f.open(inputFileName.c_str(), ifstream::in);
	currentToken = "";
	isinProcessing = false;
}
JackTokenizer::~JackTokenizer()
{
	f.close();
}

bool JackTokenizer::hasMoreTokens()
{
	if (isinProcessing)
		return true;
	if (f.eof())
	{
		return false;
	}
	bool hasMoreTokens = false;
	string s;
	istream::streampos pos = f.tellg();
	f.clear();
	do
	{
		f >> s;
		bool isMultipleComment = false;
		trimLeft(s);
		if (isOneLineComment(s))
		{
			getline(f, s);
			continue;
		}
		if (s.find("/*") == 0)
		{
			while (s.find("*/") == string::npos)
			{
				f >> s;
			}
			isMultipleComment = true;
		}

		if (!s.empty() && !isMultipleComment)
		{
			hasMoreTokens = true;
			break;
		}
	} while (!f.eof());
	f.seekg(pos); 
	return hasMoreTokens;
}
void JackTokenizer::advance()
{
	if (isinProcessing)
	{
		if (tokenItems.empty())
			isinProcessing = false;
		else
		{
			currentToken = tokenItems.front();
			tokenItems.pop();
			return;
		}
	}
	string s;
	do
	{
		f >> s;
		bool isMultipleComment = false;
		trimLeft(s);
		if (isOneLineComment(s))
		{
			getline(f, s);
			continue;
		}
		if (s.find("/*") == 0)
		{
			while (s.find("*/") == string::npos)
			{
				f >> s;
			}
			isMultipleComment = true;
		}
		if (!s.empty() && !isMultipleComment)
		{
			trimRight(s);
			currentTokens = s;
			break;
		}
	} while (!f.eof());
	processTokens();
	if (!isinProcessing)
	{
		currentToken = tokenItems.front();
		tokenItems.pop();
	}
}
Token JackTokenizer::tokenType() 
{
	if (isdigit(currentToken[0]))
		return INT_CONST;
	else if (currentToken[0] == '\"')
		return STRING_CONST;
	else if (TokenMap.count(currentToken))
		return TokenMap.at(currentToken);
	else
		return IDENTIFIER;
}
Keyword JackTokenizer::keyword() 
{
	return KeywordMap.at(currentToken);
}
char JackTokenizer::symbol()
{
	return currentToken[0];
}
string JackTokenizer::identifier() 
{
	return currentToken;
}
int JackTokenizer::intVal() 
{
	return stoi(currentToken);
}
string JackTokenizer::stringVal()
{
	return currentToken.substr(1, currentToken.length() - 2);
}

bool JackTokenizer::isOneLineComment(string& line)
{
	return (line.find("//") == 0) ;
}
void JackTokenizer::trimLeft(string& line)
{
	const string blank = " \t\n\f\v\r";
	line.erase(0, line.find_first_not_of(blank));
}
void JackTokenizer::trimRight(string& line)
{
	const string blank = " \t\n\f\v\r;";
	int commentPos = line.find("//");
	if (commentPos != 0 && commentPos != string::npos)
	{
		line.erase(line.find_first_of("//"), string::npos);
	}
	line.erase(line.find_last_not_of(blank) + 1, string::npos);
}
void JackTokenizer::processTokens()
{
	string token = "";
	for (int i = 0; i < currentTokens.size(); i++)
	{
		string tmp;
		tmp += currentTokens[i]; //char to string
		if (TokenMap.count(tmp))
		{
			if (token != "")
				tokenItems.push(token);
			tokenItems.push(tmp);
			token = "";
			continue;
		}
		if (currentTokens[i] == '\"')
		{
			token.append(currentTokens.substr(i,string::npos));
			string s = "";
			while (s.find("\"") == string::npos)
			{
				f >> s;
				currentTokens.append(s);
				token.append(" ");
				token.append(s.substr(0, s.find("\"")));
			} 
			tokenItems.push(token);
			token = "";
		}
		token += currentTokens[i];
	}
	if (token != "")
		tokenItems.push(token);
}