#include <iostream>
#include <string>
#include "JackAnalyzer.h"

using namespace std; 

int main(int argc, char* argv[])
{
	string inputPathName;
	inputPathName = argv[1];

	JackAnalyzer myAnalyzer(inputPathName);
	myAnalyzer.analyze();

	return 0;
}