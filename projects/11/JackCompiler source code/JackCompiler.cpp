#include "JackCompiler.h"

namespace fs = std::experimental::filesystem;

JackCompiler::JackCompiler(const string& pathName) :
	pathName(pathName)
{}
void JackCompiler::Compile()
{
    string inputExtension = getExtension(pathName);
    if (inputExtension == "jack")
    {
        string InputFileName = pathName;
        string className = removeExtension(pathName);
        
        CompilationEngine myComilationEngine(InputFileName, className);
        myComilationEngine.CompileFile();

    }
    /*else if (inputExtension == "")
    {
        fs::path myPath(pathName);
        for (auto& i : fs::recursive_directory_iterator(myPath))
        {
            //
            string currentInputFileName = i.path().string();
            if (getExtension(currentInputFileName) != "jack")
                continue;
            string InputFileName = currentInputFileName;
            string outputFileName = removeExtension(currentInputFileName);

            outputFileName.append(".xml");

            JackTokenizer myTokenizer(InputFileName);

        }
    }*/
}

string JackCompiler::getExtension(const string& fileName)
{
    if (fileName.find_last_of(".") == string::npos)
        return "";
    return fileName.substr(fileName.find_last_of(".") + 1);
}

string JackCompiler::removeExtension(string& fileName)
{
    return fileName.substr(0, fileName.find_last_of("."));
}