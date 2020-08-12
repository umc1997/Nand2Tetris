#include "VMtranslator.h"

VMtranslator::VMtranslator(const string& pathName)
{
	this->pathName = pathName;
	//diretory or vm file processing
}
void VMtranslator::translate(ofstream& outputFile)
{
	CodeWriter c(outputFile);
	string currentInputFileName;
	//while(file number)
	currentInputFileName = pathName;
	Parser p(currentInputFileName);
	c.setFileName(currentInputFileName);
	do
	{
		p.advance();
		switch (p.commandType())
		{
		case(C_ARITHMETIC):
			c.writerArithmetic(p.arg1());
			break;
		case(C_PUSH):
		case(C_POP):
			c.writePushPop(p.commandType(),p.arg1(),p.arg2());
			break;
		}
	} while (p.hasMoreCommands());
	//
}