#include "CodeWriter.h"

CodeWriter::CodeWriter(string& outputFileName)
:outputFileName(outputFileName),LABEL_NUMBER(0)
{
	outputFile.open(outputFileName.append(".asm"));
	currentFunctionName = "";
}
void CodeWriter::setFileName(string& filename)
{
	this->currentFileName = filename.substr(0, filename.find_last_of("."));
	currentFunctionName = "";
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
				outputFile << "@" << currentFileName << "." << to_string(index) << endl;
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
				outputFile << "@" << currentFileName << "." << to_string(index) << endl;
				outputFile << "D=A" << endl;
				outputFile << "@" << to_string(tmp) << endl;
				outputFile << "M=D" << endl;
				writePop(tmp,true);
			}
		}//symbol
	}
}
void CodeWriter::writetInit(){
	outputFile << "@256" << endl;
	outputFile << "D=A" << endl;
	outputFile << "@SP" << endl;
	outputFile << "M=D" << endl;
	writeCall("Sys.init", 0);
}
void CodeWriter::writeLabel(const string& label){
	outputFile << currentFunctionName;
	outputFile << "(" << label << ")" << endl;
}
void CodeWriter::writeGoto(const string& label){
	outputFile << "@" << currentFunctionName << label << endl;
	outputFile << "0;JMP" << endl;
}
void CodeWriter::writeIf(const string& label) {
	outputFile << "@SP" << endl;
	outputFile << "AM=M-1" << endl;
	outputFile << "D=M" << endl;
	outputFile << "@" << currentFunctionName << label << endl;
	outputFile << "D;JNE" << endl;
}
void CodeWriter::writeCall(const string& functionName, int numArgs) {
	string labelName = "return-address-of-";
	labelName.append(functionName);
	//push return-address
	outputFile << "@" << labelName << endl;
	outputFile << "D=M" << endl;
	writePush();
	//push LCL
	outputFile << "@LCL" << endl;
	outputFile << "D=M" << endl;
	writePush();
	//push ARG
	outputFile << "@ARG" << endl;
	outputFile << "D=M" << endl;
	writePush();
	//push THIS
	outputFile << "@THIS" << endl;
	outputFile << "D=M" << endl;
	writePush();
	//push THAT
	outputFile << "@THAT" << endl;
	outputFile << "D=M" << endl;
	writePush();
	//reset ARG
	outputFile << "@SP" << endl;
	outputFile << "D=M" << endl;
	outputFile << "@" << to_string(numArgs) << endl;
	outputFile << "D=D-A" << endl;
	outputFile << "@5" << endl;
	outputFile << "D=D-A" << endl;
	outputFile << "@ARG" << endl;
	outputFile << "M=D" << endl;
	//reset LCL
	outputFile << "@SP" << endl;
	outputFile << "D=M" << endl;
	outputFile << "@LCL" << endl;
	outputFile << "M=D" << endl;

	writeGoto(functionName);

	writeLabel(labelName);
}
void CodeWriter::writeReturn(){
	//TODO
	string RET = "return-address-of-";
	RET.append(currentfunctionName);
	//FRAME = LCL
	outputFile << "@LCL" << endl;
	outputFile << "D=M" << endl;
	outputFile << "@FRAME" << endl;
	outputFile << "M=D" << endl;
	//RET = *(FRAME-5)
	outputFile << "@5" << endl;
	outputFile << "A=D-A" << endl;
	outputFile << "D=M" << endl;
	outputFile << "@" << RET << endl;
	outputFile << "M=D" << endl;
	//*ARG = pop()
	writePop(2, true);
	//SP = ARG+1
	outputFile << "@ARG" << endl;
	outputFile << "D=M+1" << endl;
	outputFile << "@SP" << endl;
	outputFile << "M=D" << endl;
	//THAT = *(FRAME-1)
	outputFile << "@FRAME" << endl;
	outputFile << "AM=M-1" << endl;
	outputFile << "D=M" << endl;
	outputFile << "@THAT" << endl;
	outputFile << "M=D" << endl;
	//THIS = *(FRAME-2)
	outputFile << "@FRAME" << endl;
	outputFile << "AM=M-1" << endl;
	outputFile << "D=M" << endl;
	outputFile << "@THIS" << endl;
	outputFile << "M=D" << endl;
	//ARG = *(FRAME-3)
	outputFile << "@FRAME" << endl;
	outputFile << "AM=M-1" << endl;
	outputFile << "D=M" << endl;
	outputFile << "@ARG" << endl;
	outputFile << "M=D" << endl;
	//LCL = *(FRAME-4)
	outputFile << "@FRAME" << endl;
	outputFile << "AM=M-1" << endl;
	outputFile << "D=M" << endl;
	outputFile << "@LCL" << endl;
	outputFile << "M=D" << endl;
	//goto RET
	writeGoto(RET);
}
void CodeWriter::writeFunction(const string& functionName, int numLocals){

	writeLabel(functionName);
	currentFunctionName = functionName;
	currentFunctionName.append("$");
	for (int i = 0; i < numLocals; i++)
	{
		outputFile << "D=0" << endl;
		writePushPop(C_PUSH, "local", i);
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
//R[address] = pop()
//needAccessRAM = ?(@R[index]=address : @index = address)
void CodeWriter::writePop(int index, bool needAccessRAM){
	outputFile << "@SP" << endl;
	outputFile << "AM=M-1" << endl;
	outputFile << "D=M" << endl;
	outputFile << "M=0" << endl;
	outputFile << "@" << to_string(index) << endl;
	if (needAccessRAM)
		outputFile << "A=M" << endl;
	outputFile << "M=D" << endl;

}