#include <iostream>
#include <string>
#include "JackCompiler.h"

using namespace std;

//int main(int argc, char* argv[])
int main()
{
	string inputPathName;
	inputPathName = "Main.jack";//argv[1];

	JackCompiler myCompiler(inputPathName);
	myCompiler.Compile();

	return 0;
}
