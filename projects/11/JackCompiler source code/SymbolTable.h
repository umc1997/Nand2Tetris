#pragma once
#include "JackTokenizer.h"
#include <unordered_map>
using namespace std;

enum Range { STATIC_R, FIELD_R, ARG_R, VAR_R, NONE_R};
struct table {
	string Type;
	Range Kind;
	int Index;
};
class SymbolTable {
public:
	SymbolTable();
	void startSubroutine();
	void Define(const string& name, const string& type, Range kind);
	int VarCount(Range kind);
	Range KindOf(const string& name);
	string TypeOf(const string& name);
	int IndexOf(const string& name);
	void setCurrentClassName(const string& className);
	bool isExist(const string& name);
private:
	string currentClassName;
	unordered_map<string, table> classMap;
	unordered_map<string, table> subroutineMap;
	int classStaticIndex;
	int classFieldIndex;
	int subroutineArgumentIndex;
	int subroutineVarIndex;
};