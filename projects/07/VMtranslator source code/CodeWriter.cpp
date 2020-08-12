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
	uint16_t indexofX = 0x000D; //test
	uint16_t indexofY = 0x000E; //test

	writePop(indexofY,false);

	if (command == "neg")
	{
		outputFile << "@" << to_string(indexofY) << endl;
		outputFile << "D=-M" << endl;
	}
	else if (command == "not")
	{
		outputFile << "@" << to_string(indexofY) << endl;
		outputFile << "D=!M" << endl;
	}
	else
	{
		writePop(indexofX,false);
		if (command == "add")
		{
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D+M" << endl;
		}
		if (command == "sub")
		{
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D-M" << endl;
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
		}
		if (command == "and")
		{
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D&M" << endl;
		}
		if (command == "or")
		{
			outputFile << "@" << to_string(indexofX) << endl;
			outputFile << "D=M" << endl;
			outputFile << "@" << to_string(indexofY) << endl;
			outputFile << "D=D|M" << endl;
		}
	}
	writePush();
}
void CodeWriter::writePushPop(COMMAND command, const string& segment, int index)
{
	if (command == C_PUSH)
	{
		if (segment == "constant")
		{
			outputFile << "@" << to_string(index) << endl;
			outputFile << "D=A" << endl;
			writePush();
		}
		else{
			if (segment == "local")
			{
				outputFile << "@LCL" << endl;
				outputFile << "D=M" << endl;
				outputFile << "@" << to_string(index) << endl;
				outputFile << "A=D+A" << endl;
				outputFile << "D=M" << endl;
				writePush();
			}
			else if(segment == "argument")
			{ 
				outputFile << "@ARG" << endl;
				outputFile << "D=M" << endl;
				outputFile << "@" << to_string(index) << endl;
				outputFile << "A=D+A" << endl;
				outputFile << "D=M" << endl;
				writePush();
			}
			else if (segment == "this")
			{
				outputFile << "@THIS" << endl;
				outputFile << "D=M" << endl;
				outputFile << "@" << to_string(index) << endl;
				outputFile << "A=D+A" << endl;
				outputFile << "D=M" << endl;
				writePush();
			}
			else if (segment == "that")
			{
				outputFile << "@THAT" << endl;
				outputFile << "D=M" << endl;
				outputFile << "@" << to_string(index) << endl;
				outputFile << "A=D+A" << endl;
				outputFile << "D=M" << endl;
				writePush();
			}
			else if (segment == "pointer")
			{
				outputFile << "@" << to_string(POINTER_ADDRESS + index) << endl;
				outputFile << "D=M" << endl;
				writePush();
			}
			else if (segment == "temp")
			{
				outputFile << "@" << to_string(TEMP_ADDRESS + index) << endl;
				outputFile << "D=M" << endl;
				writePush();
			}
			else if (segment == "static")
			{
				outputFile << "@" << filename << "." << to_string(index) << endl;
				outputFile << "D=M" << endl;
				writePush();
			}
		}//symbol
	}
	if (command == C_POP)
	{
		if (segment == "constant")
		{
			//
		}
		else {
			uint16_t tmp = 0x000D;
			if (segment == "local")
			{
				outputFile << "@LCL" << endl;
				outputFile << "D=M" << endl;
				outputFile << "@" << to_string(index) << endl;
				outputFile << "D=D+A" << endl;
				outputFile << "@" << to_string(tmp) << endl;
				outputFile << "M=D" << endl;
				writePop(tmp,true);
			}
			else if (segment == "argument")
			{
				outputFile << "@ARG" << endl;
				outputFile << "D=M" << endl;
				outputFile << "@" << to_string(index) << endl;
				outputFile << "D=D+A" << endl;
				outputFile << "@" << to_string(tmp) << endl;
				outputFile << "M=D" << endl;
				writePop(tmp, true);
			}
			else if (segment == "this")
			{
				outputFile << "@THIS" << endl;
				outputFile << "D=M" << endl;
				outputFile << "@" << to_string(index) << endl;
				outputFile << "D=D+A" << endl;
				outputFile << "@" << to_string(tmp) << endl;
				outputFile << "M=D" << endl;
				writePop(tmp, true);
			}
			else if (segment == "that")
			{
				outputFile << "@THAT" << endl;
				outputFile << "D=M" << endl;
				outputFile << "@" << to_string(index) << endl;
				outputFile << "D=D+A" << endl;
				outputFile << "@" << to_string(tmp) << endl;
				outputFile << "M=D" << endl;
				writePop(tmp, true);
			}
			else if (segment == "pointer")
			{
				writePop(POINTER_ADDRESS + index, false);
			}
			else if (segment == "temp")
			{
				writePop(TEMP_ADDRESS + index, false);
			}
			else if (segment == "static")
			{
				outputFile << "@" << filename << "." << to_string(index) << endl;
				outputFile << "D=M" << endl;
				outputFile << "@" << to_string(tmp) << endl;
				outputFile << "M=D" << endl;
				writePop(tmp,true);
			}
		}//symbol
	}
}
void CodeWriter::Close() 
{
	outputFile.close();
}

// D = constant for push
void CodeWriter::writePush() {
	outputFile << "@SP" << endl;
	outputFile << "A=M" << endl;
	outputFile << "M=D" << endl;
	outputFile << "@SP" << endl;
	outputFile << "M=M+1" << endl;
}
//R[index] = pop()
//isAddress = (Is index means memory address?)
void CodeWriter::writePop(int index, bool isAddress){
	outputFile << "@SP" << endl;
	outputFile << "AM=M-1" << endl;
	outputFile << "D=M" << endl;
	outputFile << "M=0" << endl;
	outputFile << "@" << to_string(index) << endl;
	if (isAddress)
		outputFile << "A=M" << endl;
	outputFile << "M=D" << endl;

}
