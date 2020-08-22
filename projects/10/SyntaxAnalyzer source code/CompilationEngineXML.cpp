#include "CompilationEngineXML.h"

CompilationEngineXML::CompilationEngineXML(const string& inputFileName, const string& outputFileName)
    :inputFileName(inputFileName.c_str()), outputFileName(outputFileName.c_str()), myTokenizer(inputFileName.c_str())
{
    outputFile.open(this->outputFileName.c_str(), ios::out);
    tapCount = 0;
}
CompilationEngineXML::~CompilationEngineXML()
{
    outputFile.close();
    myTokenizer.close();
}

void CompilationEngineXML::CompileFileTokens()
{
    writeTagStart("tokens");
    if (!myTokenizer.hasMoreTokens()) throw("Error: It is a empty file.");
    myTokenizer.advance();
    while (myTokenizer.hasMoreTokens())
    {
        switch (myTokenizer.tokenType())
        {
        case(KEYWORD):
            CompileKeyword();
            break;
        case(IDENTIFIER):
            CompileIdentifier();
            break;
        case(SYMBOL):
            CompileSymbol();
            break;
        case(INT_CONST):
            CompileIntVal();
            break;
        case(STRING_CONST):
            CompileStringVal();
            break;
        default:
            break;
        }
    }
    CompileSymbol();
    writeTagEnd("tokens");
}
void CompilationEngineXML::CompileFile()
{
    if (!myTokenizer.hasMoreTokens()) throw("Error:It is empty file.");
    myTokenizer.advance();
    CompileClass();
}
void CompilationEngineXML::CompileClass()
{
    if (myTokenizer.tokenType() != KEYWORD || myTokenizer.keyword() != CLASS) throw("Error: Each file must contain a class.");
    writeTagStart("class");
    CompileKeyword();
    CompileIdentifier();
    if (!isBraceStart()) throw("Error: Syntax Error");
    CompileSymbol();
    while (!isBraceEnd()) {
        if (myTokenizer.keyword() == STATIC || myTokenizer.keyword() == FIELD) //STATIC || FIELD
            CompileClassVarDec();
        if (myTokenizer.keyword() == CONSTRUCTOR || myTokenizer.keyword() == FUNCTION || myTokenizer.keyword() == METHOD) //CONSTRUCTOR || FUNCTION || METHOD
            CompileSubroutine();
    }
    CompileSymbol();
    writeTagEnd("class");
}
void CompilationEngineXML::CompileClassVarDec()
{
    writeTagStart("classVarDec");
    CompileKeyword();
    // type
    CompileType();
    // varName
    do
    {
        if (isComma())
        {
            CompileSymbol();
        }
        if (myTokenizer.tokenType() == IDENTIFIER)
            CompileIdentifier();
        else; //error
    } while (!isSemiCollon());
    CompileSymbol();
    writeTagEnd("classVarDec");
}
void CompilationEngineXML::CompileSubroutine()
{
    writeTagStart("subroutineDec");
    CompileKeyword();
    // VOID || TYPE
    if (myTokenizer.tokenType() == KEYWORD && myTokenizer.keyword() == VOID)
        CompileKeyword();
    else
        CompileType();
    //SubroutineName
    if (myTokenizer.tokenType() == IDENTIFIER)
        CompileIdentifier();
    else;//error

    //(parameterlist)
    if (isParentheseStart())
        CompileSymbol();
    writeTagStart("parameterList");
    if (!isParentheseEnd())
        CompileParameterList();

    writeTagEnd("parameterList");
    CompileSymbol();

    // subroutine body
    writeTagStart("subroutineBody");
    if (!isBraceStart())throw("Error:Subroutine must have body.");
    //{
    CompileSymbol();
    while (myTokenizer.tokenType() == KEYWORD && myTokenizer.keyword() == VAR)
    {
        //VarDec
        CompileVarDec();
    }
    //statements
    CompileStatements();
    // }
    CompileSymbol();

    writeTagEnd("subroutineBody");
    writeTagEnd("subroutineDec");
}
void CompilationEngineXML::CompileParameterList()
{
    if (!isType()) throw("Error: Syntax Error");

    CompileType();
    CompileIdentifier();
    while (isComma())
    {
        CompileSymbol();
        CompileType();
        CompileIdentifier();
    }
}
void CompilationEngineXML::CompileVarDec()
{
    writeTagStart("varDec");
    CompileKeyword();
    CompileType();
    CompileIdentifier();

    while (isComma())
    {
        CompileSymbol(); //,
        CompileIdentifier();  //varName
    }
    CompileSymbol();  //;
    writeTagEnd("varDec");
}
void CompilationEngineXML::CompileStatements()
{
    writeTagStart("statements");
    do {
        switch (myTokenizer.keyword())
        {
        case(LET):
            CompileLet();
            break;
        case(IF):
            CompileIf();
            break;
        case(WHILE):
            CompileWhile();
            break;
        case(DO):
            CompileDo();
            break;
        case(RETURN):
            CompileReturn();
            break;
        default://error
            break;
        }
    } while (isStatement());

    writeTagEnd("statements");
}
void CompilationEngineXML::CompileDo()
{
    if (!isStatement()) throw("Error: Statement Error");

    writeTagStart("doStatement");
    //do
    CompileKeyword();
    //subroutineCall
    CompileSubroutineCall();

    if (!isSemiCollon()) throw("Error: Syntax Error");
    CompileSymbol();

    writeTagEnd("doStatement");

}
void CompilationEngineXML::CompileLet()
{
    if (!isStatement()) throw("Error: Statement Error");

    writeTagStart("letStatement");
    //let
    CompileKeyword();
    //varName
    CompileIdentifier();
    if (isBracketStart())
    {
        CompileSymbol();
        //[ expression
        CompileExpression();
        if (!isBracketEnd()) throw("Error: Syntax Error");
        CompileSymbol();
    }
    //=
    CompileSymbol();
    //expression
    CompileExpression();

    if (!isSemiCollon()) throw("Error: Syntax Error");
    //;
    CompileSymbol();
    writeTagEnd("letStatement");
}
void CompilationEngineXML::CompileWhile()
{
    if (!isStatement()) throw("Error: Statement Error");

    writeTagStart("whileStatement");

    CompileKeyword();
    if (!isParentheseStart())throw("Error: Syntax Error");
    CompileSymbol();
    CompileExpression();
    if (!isParentheseEnd())throw("Error: Syntax Error");
    CompileSymbol();

    if (!isBraceStart())throw("Error: Syntax Error");
    CompileSymbol();
    CompileStatements();
    if (!isBraceEnd())throw("Error: Syntax Error");
    CompileSymbol();

    writeTagEnd("whileStatement");

}
void CompilationEngineXML::CompileReturn()
{
    if (!isStatement()) throw("Error: Statement Error");

    writeTagStart("returnStatement");
    CompileKeyword();

    if (isExpression())
        CompileExpression();

    if (!isSemiCollon()) throw("Error: Syntax Error");
    CompileSymbol();
    writeTagEnd("returnStatement");

}
void CompilationEngineXML::CompileIf()
{
    if (!isStatement()) throw("Error: Statement Error");

    writeTagStart("ifStatement");

    CompileKeyword();

    if (!isParentheseStart())throw("Error: Syntax Error");
    CompileSymbol();
    CompileExpression();
    if (!isParentheseEnd())throw("Error: Syntax Error");
    CompileSymbol();

    if (!isBraceStart())throw("Error: Syntax Error");
    CompileSymbol();
    CompileStatements();
    if (!isBraceEnd())throw("Error: Syntax Error");
    CompileSymbol();

    if (myTokenizer.tokenType() == KEYWORD && myTokenizer.keyword() == ELSE)
    {
        CompileKeyword();

        if (!isBraceStart())throw("Error: Syntax Error");
        CompileSymbol();
        CompileStatements();
        if (!isBraceEnd())throw("Error: Syntax Error");
        CompileSymbol();
    }

    writeTagEnd("ifStatement");

}
void CompilationEngineXML::CompileExpression()
{
    if (!isExpression()) throw("Error: Expression Error");
    writeTagStart("expression");
    CompileTerm();

    while (isOperator())
    {
        CompileSymbol();
        CompileTerm();
    }

    writeTagEnd("expression");
}
void CompilationEngineXML::CompileTerm()
{
    if (!isTerm()) throw("Error: Term Error");

    writeTagStart("term");

    switch (myTokenizer.tokenType())
    {
    case(INT_CONST):
        CompileIntVal();
        break;
    case(STRING_CONST):
        CompileStringVal();
        break;
    case(IDENTIFIER):
        //varName,varName[],subroutineCall
        CompileIdentifier();
        //varName[]
        if (isBracketStart())
        {
            CompileSymbol();
            CompileExpression();
            if (!isBracketEnd()) throw("Error: Syntax Error");
            CompileSymbol();
        }
        //subroutineCall
        else if (isParentheseStart() || isDot())
        {
            CompileSubroutineCallwithoutName();
        }
        //varName
        break;
    case(KEYWORD):
        if (isKeywordConstant()) CompileKeyword();
        break;
    case(SYMBOL):
        if (isParentheseStart()) {
            CompileSymbol();
            CompileExpression();
            if (!isParentheseEnd()) throw("Error: Syntax Error");
            CompileSymbol();
        }
        else if (isUnaryOperator())
        {
            CompileSymbol();
            CompileTerm();
        }
        else; //error
        break;
    }
    writeTagEnd("term");
}
void CompilationEngineXML::CompileExpressionList()
{
    if (!isExpression())
    {
        writeTagStart("expressionList");
        writeTagEnd("expressionList");
        return;
    }
    writeTagStart("expressionList");
    CompileExpression();
    while (isComma())
    {
        CompileSymbol();
        CompileExpression();
    }
    writeTagEnd("expressionList");
}

void CompilationEngineXML::CompileType()
{
    if (!isType()) throw("Error: Syntax Error");
    if (myTokenizer.tokenType() == KEYWORD)
    {
        if (myTokenizer.keyword() == INT)
            CompileKeyword();
        else if (myTokenizer.keyword() == CHAR)
            CompileKeyword();
        else if (myTokenizer.keyword() == BOOLEAN)
            CompileKeyword();
    }
    else if (myTokenizer.tokenType() == IDENTIFIER)
        CompileIdentifier();
    else; // error
}
void CompilationEngineXML::CompileSubroutineCall()
{
    CompileIdentifier();
    CompileSubroutineCallwithoutName();
}
void CompilationEngineXML::CompileSubroutineCallwithoutName()
{
    if (!isParentheseStart() && !isDot()) throw("Error: Syntax Error");

    if (isParentheseStart())
    {
        CompileSymbol();
        CompileExpressionList();
        if (!isParentheseEnd()) throw("Error: Syntax Error");
        CompileSymbol();
    }
    else if (isDot())
    {
        CompileSymbol();
        CompileIdentifier();
        if (!isParentheseStart())throw("Error: Syntax Error");
        CompileSymbol();
        CompileExpressionList();
        if (!isParentheseEnd()) throw("Error: Syntax Error");
        CompileSymbol();
    }
}
void CompilationEngineXML::CompileKeyword() {

    if (myTokenizer.tokenType() != KEYWORD) throw ("Error: Syntax Error: Keyword.");
    writeKeyword(myTokenizer.currentToken);
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
}
void CompilationEngineXML::CompileIdentifier() {
    if (myTokenizer.tokenType() != IDENTIFIER) throw ("Error: Syntax Error: Identifier.");
    writeIdentifier(myTokenizer.identifier());
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
}
void CompilationEngineXML::CompileSymbol() {
    if (myTokenizer.tokenType() != SYMBOL) throw ("Error: Syntax Error: Symbol.");
    writeSymbol(myTokenizer.symbol());
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
}
void CompilationEngineXML::CompileIntVal() {
    if (myTokenizer.tokenType() != INT_CONST) throw ("Error: Syntax Error: Integer.");
    writeIntVal(myTokenizer.intVal());
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
}
void CompilationEngineXML::CompileStringVal() {
    if (myTokenizer.tokenType() != STRING_CONST) throw ("Error: Syntax Error: String.");
    writeStringVal(myTokenizer.stringVal());
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
}


void CompilationEngineXML::writeTagStart(const string& tag)
{
    outputFile << tapAppend() << "<" << tag << ">" << endl;
    tapCount++;
}
void CompilationEngineXML::writeTagEnd(const string& tag)
{
    tapCount--;
    outputFile << tapAppend() << "</" << tag << ">" << endl;
}
void CompilationEngineXML::writeKeyword(const string& s)
{
    outputFile << tapAppend() << "<keyword>" << s << "</keyword>" << endl;
}
void CompilationEngineXML::writeIdentifier(const string& s)
{
    outputFile << tapAppend() << "<identifier>" << s << "</identifier>" << endl;
}
void CompilationEngineXML::writeSymbol(const char c)
{
    string tmp;
    tmp += c;
    if (tmp == "<")
        tmp = "&lt;";
    else if (tmp == ">")
        tmp = "&gt;";
    else if (tmp == "\"")
        tmp = "&quot;";
    else if (tmp == "&")
        tmp = "&amp;";
    outputFile << tapAppend() << "<symbol>" << tmp << "</symbol>" << endl;
}
void CompilationEngineXML::writeIntVal(const int i)
{
    outputFile << tapAppend() << "<integerConstant>" << to_string(i) << "</integerConstant>" << endl;
}
void CompilationEngineXML::writeStringVal(const string& s)
{
    outputFile << tapAppend() << "<stringConstant>" << s << "</stringConstant>" << endl;
}

string CompilationEngineXML::tapAppend()
{
    string t = "";
    for (int i = 0; i < tapCount; i++)
        t.append("\t");
    return t;
}

bool CompilationEngineXML::isExpression() {
    return isTerm();
}
bool CompilationEngineXML::isTerm() {
    switch (myTokenizer.tokenType())
    {
    case(INT_CONST):
    case(STRING_CONST):
    case(IDENTIFIER):
        return true;
        break;
    case(KEYWORD):
        return isKeywordConstant();
        break;
    case(SYMBOL):
        return (isParentheseStart() || isUnaryOperator());
        break;
    default:
        return false;
    }
}
bool CompilationEngineXML::isKeywordConstant() {
    return ((myTokenizer.tokenType() == KEYWORD) &&
        (myTokenizer.keyword() == TRUE ||
            myTokenizer.keyword() == FALSE ||
            myTokenizer.keyword() == JACK_NULL ||
            myTokenizer.keyword() == THIS));
}
bool CompilationEngineXML::isUnaryOperator() {
    return ((myTokenizer.tokenType() == SYMBOL) &&
        (myTokenizer.symbol() == '-' || myTokenizer.symbol() == '~'));
}
bool CompilationEngineXML::isOperator() {
    return ((myTokenizer.tokenType() == SYMBOL)
        && (myTokenizer.symbol() == '+'
            || myTokenizer.symbol() == '-'
            || myTokenizer.symbol() == '*'
            || myTokenizer.symbol() == '/'
            || myTokenizer.symbol() == '&'
            || myTokenizer.symbol() == '|'
            || myTokenizer.symbol() == '<'
            || myTokenizer.symbol() == '>'
            || myTokenizer.symbol() == '='));
}
bool CompilationEngineXML::isSubroutineCall() {
    return (myTokenizer.tokenType() == IDENTIFIER);
}
bool CompilationEngineXML::isParentheseStart() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == '(');
}
bool CompilationEngineXML::isParentheseEnd() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == ')');
}
bool CompilationEngineXML::isBraceStart() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == '{');
}
bool CompilationEngineXML::isBraceEnd() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == '}');
}
bool CompilationEngineXML::isBracketStart() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == '[');
}
bool CompilationEngineXML::isBracketEnd() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == ']');
}
bool CompilationEngineXML::isComma() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == ',');
}
bool CompilationEngineXML::isDot()
{
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == '.');
}
bool CompilationEngineXML::isSemiCollon() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == ';');
}
bool CompilationEngineXML::isStatement()
{
    return((myTokenizer.tokenType() == KEYWORD) &&
        (myTokenizer.keyword() == LET ||
            myTokenizer.keyword() == IF ||
            myTokenizer.keyword() == WHILE ||
            myTokenizer.keyword() == DO ||
            myTokenizer.keyword() == RETURN)
        );
}
bool CompilationEngineXML::isType() {
    switch (myTokenizer.tokenType())
    {
    case(KEYWORD):
        if (myTokenizer.keyword() == INT) return true;
        else if (myTokenizer.keyword() == CHAR) return true;
        else if (myTokenizer.keyword() == BOOLEAN) return true;
        break;
    case(IDENTIFIER):
        return true;
        break;
    default: //error
        return false;
        break;
    }
}