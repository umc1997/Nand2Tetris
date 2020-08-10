#include <iostream>
#include <fstream>
#include "Assembler.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc <= 1 || argc > 3) {
		cout << "Error, It needs 2 argv for assembler." << endl;
		return -1;
	}
	string outputFilename;
	ofstream outputFile;
	if (argc == 3)
	{
		outputFilename = argv[2];
		outputFile.open(outputFilename, ifstream::out);
	}
	string inputFilename = argv[1];
	SymbolTable SymbolTable;
	Assembler myAssembler(inputFilename,SymbolTable);
	myAssembler.Assembly(outputFile);
	outputFile.close();

	return 0;
}