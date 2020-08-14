#include "VMtranslator.h"

namespace fs = std::experimental::filesystem;

VMtranslator::VMtranslator(const string& pathName)
{
	this->pathName = pathName;
}
void VMtranslator::translate()
{

	string currentInputFileName;
	string inputExtension = getExtension(pathName);
	if (inputExtension == "vm")
	{
		string s = removeExtension(pathName);
		CodeWriter c(s);
		c.writetInit();
		currentInputFileName = pathName;
		Parser p(currentInputFileName);
		c.setFileName(currentInputFileName);
		while (p.hasMoreCommands())
		{
			p.advance();
			switch (p.commandType())
			{
			case(C_ARITHMETIC):
				c.writerArithmetic(p.arg1());
				break;
			case(C_PUSH):
			case(C_POP):
				c.writePushPop(p.commandType(), p.arg1(), p.arg2());
				break;
			case(C_LABEL):
				c.writeLabel(p.arg1());
				break;
			case(C_GOTO):
				c.writeGoto(p.arg1());
				break;
			case(C_IF):
				c.writeIf(p.arg1());
				break;
			case(C_CALL):
				c.writeCall(p.arg1(), p.arg2());
				break;
			case(C_RETURN):
				c.writeReturn();
				break;
			case(C_FUNCTION):
				c.writeFunction(p.arg1(), p.arg2());
				break;
			}
		}

		c.Close();
	}
	else if (inputExtension == "")
	{
		fs::path myPath(pathName);
		string s = myPath.string();
		CodeWriter c(s);
		c.writetInit();
		for (auto& i : fs::recursive_directory_iterator(myPath))
		{
			currentInputFileName = i.path().filename().string();
			if (getExtension(currentInputFileName) != "vm")
				continue;
			Parser p(i.path().string()); 
			c.setFileName(currentInputFileName);
			while (p.hasMoreCommands())
			{
				p.advance();
				switch (p.commandType())
				{
				case(C_ARITHMETIC):
					c.writerArithmetic(p.arg1());
					break;
				case(C_PUSH):
				case(C_POP):
					c.writePushPop(p.commandType(), p.arg1(), p.arg2());
					break;
				case(C_LABEL):
					c.writeLabel(p.arg1());
					break;
				case(C_GOTO):
					c.writeGoto(p.arg1());
					break;
				case(C_IF):
					c.writeIf(p.arg1());
					break;
				case(C_CALL):
					c.writeCall(p.arg1(), p.arg2());
					break;
				case(C_RETURN):
					c.writeReturn();
					break;
				case(C_FUNCTION):
					c.writeFunction(p.arg1(), p.arg2());
					break;
				}
			} 

		}
		c.Close();
	}
	else
	{
		cout << "Error: Extension is wrong" << endl;
	}
	//while(file number)
	
	//
}

string VMtranslator::getExtension(const string& fileName)
{
	if (fileName.find_last_of(".") == string::npos)
		return "";
	return fileName.substr(fileName.find_last_of(".") + 1);
}

string VMtranslator::removeExtension(string& fileName)
{
	return fileName.substr(0, fileName.find_last_of("."));
}