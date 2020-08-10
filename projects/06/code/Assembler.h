#pragma once
#include <string>
#include <bitset>
#include "Parser.h"
#include "SymbolTable.h"

using namespace std;
class Assembler
{
public:
	Assembler(const string& inputFilename, SymbolTable& mySymbolTable);
	void Assembly(ostream& outputFile) const;
	SymbolTable& mySymbolTable;

private:
	string inputFilename;
};