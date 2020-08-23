#pragma once
#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"
#include "CompilationEngine.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <iostream>
#include <ostream>

using namespace std;

class JackCompiler {
public:
	JackCompiler(const string& pathName);
	void Compile();
private:
	string pathName;
	string getExtension(const string& fileName);
	string removeExtension(string& fileName);
};