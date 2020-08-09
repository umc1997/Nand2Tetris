#pragma once
#include <string>
#include <bitset>

using namespace std;
class Assembler
{
public:
	Assembler(const string& inputFilename);
	void Assembly(ostream& outputFile) const;
private:
	string inputFilename;
};