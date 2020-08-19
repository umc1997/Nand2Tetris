#include "JackAnalyzer.h"

namespace fs = std::experimental::filesystem;

JackAnalyzer::JackAnalyzer(const string& pathName)
	:pathName(pathName)
{}
void JackAnalyzer::analyze()
{
	string inputExtension = getExtension(pathName);
	if (inputExtension == "jack")
	{
		//process
		string currentInputFileName = pathName;
		JackTokenizer t(currentInputFileName);
		while (t.hasMoreTokens())
		{
			t.advance();
			switch (t.tokenType())
			{
			case(KEYWORD):
				cout << "This is keyword: " << t.currentToken << endl;
				break;
			case(SYMBOL):
				cout << "This is symbol: " << t.symbol() << endl;
				break;
			case(IDENTIFIER):
				cout << "This is indentifier: " << t.identifier() << endl;
				break;
			case(INT_CONST):
				cout << "This is constant int: " << to_string(t.intVal()) << endl;
				break;
			case(STRING_CONST):
				cout << "This is constant string: " << t.stringVal()<< endl;
				break;
			}
		}
	}/*
	else if (inputExtension == "")
	{
		fs::path myPath(pathName);
		string s = myPath.string();
		CompilationEngine c(s);
		for (auto& i : fs::recursive_directory_iterator(myPath))
		{
			//process
			JackTokenizer t(currentInputFileName);
			while (t.hasMoreTokens())
			{
				t.advance();
			}
		}
	}*/
}

string JackAnalyzer::getExtension(const string& fileName)
{
	if (fileName.find_last_of(".") == string::npos)
		return "";
	return fileName.substr(fileName.find_last_of(".") + 1);
}

string JackAnalyzer::removeExtension(string& fileName)
{
	return fileName.substr(0, fileName.find_last_of("."));
}