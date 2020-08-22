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
        string InputFileName = pathName;
        string outputFileName = removeExtension(pathName);
        outputFileName.append(".xml");

        CompilationEngineXML c1(InputFileName, outputFileName);
        c1.CompileFile();

        string outputTokensFileName = removeExtension(pathName);;
        outputTokensFileName.append("T.xml");

        CompilationEngineXML c2(InputFileName, outputTokensFileName);
        c2.CompileFileTokens();

    }
    else if (inputExtension == "")
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
          CompilationEngineXML c(InputFileName, outputFileName);
          c.CompileFile();

          string outputTokensFileName = removeExtension(currentInputFileName);;
          outputTokensFileName.append("T.xml");
          CompilationEngineXML c2(InputFileName, outputTokensFileName);
          c2.CompileFileTokens();
       }
    }
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