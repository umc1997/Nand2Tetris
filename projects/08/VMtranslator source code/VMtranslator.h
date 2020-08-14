#pragma once
#include "Parser.h"
#include "CodeWriter.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <iostream>

using namespace std;

class VMtranslator 
{
public:
	VMtranslator(const string& pathName);
	void translate();


private:
	string pathName;
	string getExtension(const string& fileName);
	string removeExtension(string& fileName);

};