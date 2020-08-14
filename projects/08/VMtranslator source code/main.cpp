#include <iostream>
#include <fstream>
#include "VMtranslator.h"

using namespace std;

int main(int argc, char* argv[]) 
{
	string inputPathName;
	inputPathName = argv[1];

	VMtranslator myVM(inputPathName);
	myVM.translate();

	return 0;
}
