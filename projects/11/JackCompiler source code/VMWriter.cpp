#include "VMWriter.h"


VMWriter::VMWriter(const string& inputFileName)
	:inputFileName(inputFileName) {
	outputFileName = inputFileName.substr(0, inputFileName.find_last_of("."));
	outputFileName = outputFileName + ".vm";
	outputFile.open(outputFileName, ios::out);
}
void VMWriter::writePush(const string& segment, int Index) {
	outputFile << "push ";
	outputFile << segment << " ";
	outputFile << to_string(Index) << endl;
}
void VMWriter::writePush(Range r, int Index) {
	outputFile << "push ";
	switch (r)
	{
	case(ARG_R):
		outputFile << "argument " ;
		break;
	case(VAR_R):
		outputFile << "local ";
		break;
	case(STATIC_R):
		//TODO
		outputFile << "static ";
		break;
	case(FIELD_R):
		outputFile << "this ";
		break;
	default://error
		break;
	}
	outputFile << to_string(Index) << endl;
}
void VMWriter::writePop(const string& segment, int Index) {
	outputFile << "pop ";
	outputFile << segment << " ";
	outputFile << to_string(Index) << endl;
}
void VMWriter::writePop(Range r, int Index) {
	outputFile << "pop ";
	switch (r)
	{
	case(ARG_R):
		outputFile << "argument ";
		break;
	case(VAR_R):
		outputFile << "local ";
		break;
	case(STATIC_R):
		//TODO
		outputFile << "static "; 
		break;
	case(FIELD_R):
		outputFile << "this ";
		break;
	default://error
		break;
	}
	outputFile << to_string(Index) << endl;
}
void VMWriter::writerArithmetic(const string& command) {
	outputFile << command << endl;
}
void VMWriter::writeLabel(const string& label) {
	outputFile << "label " << label << endl;
}
void VMWriter::writeGoto(const string& label) {
	outputFile << "goto " << label << endl;
}
void VMWriter::writeIf(const string& label) {
	outputFile << "if-goto " << label << endl;
}
void VMWriter::writeCall(const string& name, int nArgs) {
	outputFile << "call " << name << " " << to_string(nArgs) << endl;
}
void VMWriter::writeFunction(const string& name, int nLocals) {
	outputFile << "function " << name << " " << to_string(nLocals) << endl;
}
void VMWriter::writeReturn() {
	outputFile << "return" << endl;
}
void VMWriter::close() {
	outputFile.close();
}
