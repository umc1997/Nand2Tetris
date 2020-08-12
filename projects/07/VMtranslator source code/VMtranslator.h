#pragma once
#include "Parser.h"
#include "CodeWriter.h"

using namespace std;

class VMtranslator 
{
public:
	VMtranslator(const string& pathName);
	void translate(ofstream& outputFile);

private:
	string pathName;
	
};