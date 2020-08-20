#pragma once
#include "CompilationEngineXML.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <iostream>
#include <iostream>

using namespace std;

class JackAnalyzer {
public:
	JackAnalyzer(const string& pathName);
	void analyze();

private:
	string pathName;
	string getExtension(const string& fileName);
	string removeExtension(string& fileName);
};