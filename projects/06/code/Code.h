#pragma once

#include <string>
#include <unordered_map>

using namespace std;

class Code
{
public:
	Code();
	string dest(string& s);
	string comp(string& s);
	string jump(string& s);
private:
	unordered_map<string, string> destMap;
	unordered_map<string, string> compMap;
	unordered_map<string, string> jumpMap;
};