#include "Assembler.h"

Assembler::Assembler(const string& inputFilename, SymbolTable& mySymbolTable)
:mySymbolTable(mySymbolTable)
{
	this->inputFilename = inputFilename;

	Parser myParser1(inputFilename);
	Parser myParser2(inputFilename);

	uint16_t ROMaddress = 0x0;
	uint16_t RAMaddress = 0x10;
	
	//1-pass
	while (myParser1.hasMoreCommands())
	{

		myParser1.advance();
		Command commandType = myParser1.commandType();

		if (commandType == A_COMMAND || commandType == C_COMMAND)
		{
			ROMaddress++;
		}
		if (commandType == L_COMMAND)
		{
			mySymbolTable.addEntry(myParser1.symbol(), ROMaddress);
		}
	}
	//2-pass
	while (myParser2.hasMoreCommands())
	{
		myParser2.advance();
		if (myParser2.commandType() == A_COMMAND)
		{
			string s = myParser2.symbol();
			if (!isdigit(s.front()) && !mySymbolTable.contains(s))
			{
				mySymbolTable.addEntry(s, RAMaddress);
				RAMaddress++;
			}
		}
	}
}

void Assembler::Assembly(ostream& outputFile) const
{
	Parser p(inputFilename);
	Code c;

	while (p.hasMoreCommands())
	{
		p.advance();
		Command commandType = p.commandType();
		switch (commandType)
		{
			case(A_COMMAND):
			{
				string s = p.symbol();
				bitset<16> finalbit;
				//digit
				if (isdigit(s.front()))
				{
					finalbit = stoi(s);
				}
				//symbol
				else
				{
					finalbit = mySymbolTable.GetAddress(s);
				}
				outputFile << bitset<16>(finalbit) << endl;
				break;
			}
			case(C_COMMAND): {
				bitset<16> finalbit;

				bitset<16>  dest = c.dest(p.dest());
				bitset<16>  comp = c.comp(p.comp());
				bitset<16>  jump = c.jump(p.jump());
			
				finalbit = comp | dest | jump;

				outputFile << finalbit << endl;
				break;
			}
			case(L_COMMAND):
				break;
		}

	}
}

