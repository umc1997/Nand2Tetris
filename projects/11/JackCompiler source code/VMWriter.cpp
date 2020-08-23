#include "VMWriter.h"


VMWriter::VMWriter(const string& outputFileName)
	:outputFileName(outputFileName) {
	this->outputFileName = outputFileName + ".vm";
	outputFile.open(this->outputFileName, ios::out);
}
void VMWriter::writePush(const string& segment, int Index) {
	outputFile << "push ";
	outputFile << segment << " ";
	outputFile << to_string(Index) << endl;
}
void VMWriter::writePop(const string& segment, int Index) {
	outputFile << "pop ";
	outputFile << segment << " ";
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
