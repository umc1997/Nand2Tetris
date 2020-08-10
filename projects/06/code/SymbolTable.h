#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

class SymbolTable
{
public:
	SymbolTable();
	void addEntry(const string& symbol,uint16_t address);
	bool contains(const string& symbol);
	uint16_t GetAddress(const string& symbol);
private:
	unordered_map<string, uint16_t> SymbolMap;
};