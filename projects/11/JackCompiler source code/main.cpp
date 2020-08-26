#include <iostream>
#include <string>
#include "JackCompiler.h"

using namespace std;


int main(int argc, char* argv[])
{
	string inputPathName;
	inputPathName = argv[1];

	JackCompiler myCompiler(inputPathName);
	myCompiler.Compile();

	return 0;
}
