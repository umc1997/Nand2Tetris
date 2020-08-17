#pragma once
#include "JackTokenizer.h"
#include "CompilationEngine.h"
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