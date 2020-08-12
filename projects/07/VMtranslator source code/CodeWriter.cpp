#include "CodeWriter.h"

CodeWriter::CodeWriter(ofstream& outputFile)
:outputFile(outputFile)
{}
void CodeWriter::setFileName(string& filename)
{
	outputFile << "//Start translating a new vm File: " << filename << endl;
}
void CodeWriter::writerArithmetic(const string& command) 
{
	uint16_t result;
	uint16_t indexofX = 0x0005; //test
	uint16_t indexofY = 0x0006; //test
	uint16_t y = Stack.top();
	Stack.pop();
	writePop(indexofY);

	if (command == "neg")
	{
		result = ~y + 1;
		outputFile << "@" << to_string(indexofY) << endl;
		outputFile << "M=-M" << endl;
	}
	else if (command == "not")
	{
		result = ~y;
		outputFile << "@" << to_string(indexofY) << endl;
		outputFile << "M=!M" << endl;
	}
	else
	{
		uint16_t x = Stack.top();
		Stack.pop();
		writePop(indexofX);
		if (command == "add")
		{
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D+M" << endl;
			result = x + y;
		}
		if (command == "sub")
		{
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D-M" << endl;
			result = x - y;
		}
		if (command == "eq")
		{
			if (x == y)
				result = 0xFFFF;
			else
				result = 0x0000;
		}
		if (command == "gt")
		{
			if (x > y)
				result = 0xFFFF;
			else
				result = 0x0000;
		}
		if (command == "lt")
		{
			if (x < y)
				result = 0xFFFF;
			else
				result = 0x0000;
		}
		if (command == "and")
		{
			result = x & y;
		}
		if (command == "or")
		{
			result = x | y;
		}
	}
	Stack.push(result);
	writePush();
}
void CodeWriter::writePushPop(COMMAND command, const string& segment, int index)
{
	if (command == C_PUSH)
	{
		if (segment == "constant")
		{
			Stack.push(index);
			outputFile << "@" << to_string(index) << endl;
			outputFile << "D=A" << endl;
			writePush();
		}
		else{}//symbol
	}
	if (command == C_POP)
	{
		if (segment == "constant")
		{
			uint16_t pop = Stack.top();
			Stack.pop();
			writePop(index);


		}
		else {} //symbol
	}
}
void CodeWriter::Close() 
{
	outputFile.close();
}

void CodeWriter::writePush() {
	outputFile << "@SP" << endl;
	outputFile << "A=M" << endl;
	outputFile << "M=D" << endl;
	outputFile << "@SP" << endl;
	outputFile << "M=M+1" << endl;
}
//R[index] = pop()
void CodeWriter::writePop(int index){
	outputFile << "@SP" << endl;
	outputFile << "AM=M-1" << endl;
	outputFile << "D=M" << endl;

	outputFile << "@" << to_string(index) << endl;
	outputFile << "M=D" << endl;

}
