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
		string InputFileName = pathName;
		string outputFileName = removeExtension(pathName);
		outputFileName.append(".xml");

		CompilationEngine c(InputFileName,outputFileName);
		c.CompileFile();
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