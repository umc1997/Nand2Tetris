#include "Assembler.h"
#include "Parser.h"

Assembler::Assembler(const string& inputFilename)
{
	this->inputFilename = inputFilename;
}

void Assembler::Assembly(ostream& outputFile) const
{
	Parser myParser(inputFilename);
	// translate
	while (myParser.hasMoreCommands())
	{
		myParser.advance();
		Command commandType = myParser.commandType();
		string finalCode = "";
		if (commandType == A_COMMAND)
		{
			finalCode.append("0");
			string s = myParser.symbol();
			int digit = atoi(s.c_str());
			//digit
			if (digit != 0 || s.compare("0") == 0)
			{
				bitset<15> bit(digit);
				finalCode.append(bit.to_string());
			}
			//symbol
			else
			{
				finalCode.append("Need to modify");
				//Need to modify
			}
		}
		else if (commandType == C_COMMAND)
		{
			finalCode.append("111");
			Code myCode;
			string destString;
			string destCode;
			string a = "0";
			string compString;
			string compCode;
			string jumpString;
			string jumpCode;
			//dest
			destString = myParser.dest();
			destCode = myCode.dest(destString);
			//comp
			compString = myParser.comp();
			if (compString.find("M") != string::npos)
			{
				a = "1";
			}
			compCode = myCode.comp(compString);
			//jump
			jumpString = myParser.jump();
			jumpCode = myCode.jump(jumpString);
			finalCode.append(a + compCode + destCode + jumpCode);

		}
		else if (commandType == L_COMMAND)
		{
			//Need to Modify
		}
		outputFile << finalCode << endl;
		cout << finalCode << endl;
		//write in file
	}
	
}