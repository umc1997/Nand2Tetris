#include "SymbolTable.h"

SymbolTable::SymbolTable()
	 :classStaticIndex(0),
	classFieldIndex(0),
	subroutineArgumentIndex(0),
	subroutineVarIndex(0) 
{}

void SymbolTable::startSubroutine() {
	subroutineMap.clear();
	subroutineArgumentIndex = 0;
	subroutineVarIndex = 0;
	Define("this", currentClassName, ARG_R);
}
void SymbolTable::Define(const string& name, const string& type, Range kind) {
	table t;
	t.Type = type;
	t.Kind = kind;
	switch (kind)
	{
	case(STATIC_R):
		t.Index = classStaticIndex++;
		classMap.insert(make_pair(name, t));
		break;
	case(FIELD_R):
		t.Index = classFieldIndex++;
		classMap.insert(make_pair(name, t));
		break;
	case(ARG_R):
		t.Index = subroutineArgumentIndex++;
		subroutineMap.insert(make_pair(name, t));
		break;
	case(VAR_R):
		t.Index = subroutineVarIndex++;
		subroutineMap.insert(make_pair(name, t));
		break;
	default:
		break;
	}
}
int SymbolTable::VarCount(Range kind) {
	switch (kind)
	{
	case STATIC_R:
		return classStaticIndex;
		break;
	case FIELD_R:
		return classFieldIndex;
		break;
	case ARG_R:
		return subroutineArgumentIndex;
		break;
	case VAR_R:
		return subroutineVarIndex;
		break;
	case NONE_R:
		break;
	default:
		break;
	}
}
Range SymbolTable::KindOf(const string& name) {

	if (subroutineMap.count(name))
		return subroutineMap.at(name).Kind;
	else if (classMap.count(name))
		return classMap.at(name).Kind;
	else
		return NONE_R;
}
string SymbolTable::TypeOf(const string& name) {

	if (subroutineMap.count(name))
		return subroutineMap.at(name).Type;
	else if (classMap.count(name))
		return classMap.at(name).Type;
	else;//error
}
int SymbolTable::IndexOf(const string& name) {
	if (subroutineMap.count(name))
		return subroutineMap.at(name).Index;
	else if (classMap.count(name))
		return classMap.at(name).Index;
	else;//error
}
void SymbolTable::setCurrentClassName(const string& className)
{
	currentClassName = className;
}