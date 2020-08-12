#include <iostream>
#include <fstream>
#include "VMtranslator.h"

using namespace std;

//int main(int argc, char* argv[]) 
int main()
{
	string inputPathName;

	string outputFileName;
	ofstream outputFile;
	//cout << argc << endl;

	inputPathName = "Test.vm";//argv[1];
	outputFileName = "Test.asm"; //argv[2];
	outputFile.open(outputFileName, ifstream::out);

	VMtranslator myVM(inputPathName);

	myVM.translate(outputFile);

	return 0;
}
// input system 