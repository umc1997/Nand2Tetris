#include "CodeWriter.h"

CodeWriter::CodeWriter(ofstream& outputFile)
:outputFile(outputFile),LABEL_NUMBER(0)
{
}
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
		outputFile << "D=-M" << endl;
	}
	else if (command == "not")
	{
		result = ~y;
		outputFile << "@" << to_string(indexofY) << endl;
		outputFile << "D=!M" << endl;
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
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D-M" << endl;
			outputFile << "@EQ" << to_string(LABEL_NUMBER) << endl;
			outputFile << "D;JEQ" << endl;
			outputFile << "D=0" << endl;
			outputFile << "@END" << to_string(LABEL_NUMBER) << endl;
			outputFile << "0;JMP" << endl;
			outputFile << "(EQ" << to_string(LABEL_NUMBER) << ")" << endl;
			outputFile << "D=-1" << endl;
			outputFile << "(END" << to_string(LABEL_NUMBER) << ")" << endl;
			LABEL_NUMBER++;
			if (x == y)
				result = 0xFFFF;
			else
				result = 0x0000;
		}
		if (command == "gt")
		{
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D-M" << endl;
			outputFile << "@GT" << to_string(LABEL_NUMBER) << endl;
			outputFile << "D;JGT" << endl;
			outputFile << "D=0" << endl;
			outputFile << "@END" << to_string(LABEL_NUMBER) << endl;
			outputFile << "0;JMP" << endl;
			outputFile << "(GT" << to_string(LABEL_NUMBER) << ")" << endl;
			outputFile << "D=-1" << endl;
			outputFile << "(END" << to_string(LABEL_NUMBER) << ")" << endl;
			LABEL_NUMBER++;
			if (x > y)
				result = 0xFFFF;
			else
				result = 0x0000;
		}
		if (command == "lt")
		{
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D-M" << endl;
			outputFile << "@LT" << to_string(LABEL_NUMBER) << endl;
			outputFile << "D;JLT" << endl;
			outputFile << "D=0" << endl;
			outputFile << "@END" << to_string(LABEL_NUMBER) << endl;
			outputFile << "0;JMP" << endl;
			outputFile << "(LT" << to_string(LABEL_NUMBER) << ")" << endl;
			outputFile << "D=-1" << endl;
			outputFile << "(END" << to_string(LABEL_NUMBER) << ")" << endl;
			LABEL_NUMBER++;
			if (x < y)
				result = 0xFFFF;
			else
				result = 0x0000;
		}
		if (command == "and")
		{
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D&M" << endl;
			result = x & y;
		}
		if (command == "or")
		{
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D|M" << endl;
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
