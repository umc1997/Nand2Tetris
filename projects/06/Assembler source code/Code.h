#pragma once

#include <string>
#include <unordered_map>

using namespace std;

class Code
{
public:
	Code();
	uint8_t dest(const string& s);
	uint16_t comp(const string& s);
	uint8_t jump(const string& s);
private:
	unordered_map<string, uint8_t> destMap;
	unordered_map<string, uint16_t> compMap;
	unordered_map<string, uint8_t> jumpMap;
};