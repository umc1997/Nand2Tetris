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
        string className = removeExtension(pathName);
        
        CompilationEngine myComilationEngine(className, className);
        myComilationEngine.CompileFile();

    }
    else if (inputExtension == "")
    {
        fs::path myPath(pathName);
        for (auto& i : fs::recursive_directory_iterator(myPath))
        {
            //"filename.ex"
            string s = i.path().filename().string();
            if (getExtension(s) != "jack")
                continue;
            //"filename"
            string className = removeExtension(s);
            // "folder\filename.ex"
            string inputFileName = i.path().string();
            CompilationEngine myComilationEngine(inputFileName, className);
            myComilationEngine.CompileFile();

        }
    }
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