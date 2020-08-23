#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(const string& inputFileName, const string& className)
    :inputFileName(inputFileName.c_str()), 
    className(className), 
    myTokenizer(inputFileName.c_str()),
    mySymbolTable(),
    myVMWriter(className)
{}
CompilationEngine::~CompilationEngine()
{
    myTokenizer.close();
    myVMWriter.close();
}

void CompilationEngine::CompileFile()
{
    if (!myTokenizer.hasMoreTokens()) throw("Error:It is empty file.");
    myTokenizer.advance();
    CompileClass();
}
void CompilationEngine::CompileClass()
{
    if (myTokenizer.tokenType() != KEYWORD || myTokenizer.keyword() != CLASS) throw("Error: Each file must contain a class.");
    CompileSymbol();
    if (myTokenizer.identifier() != className) throw("Error: File name doesn't match class name");
    mySymbolTable.setCurrentClassName(CompileIdentifier());
    if (!isBraceStart()) throw("Error: Syntax Error");
    while (!isBraceEnd()) {
        if (myTokenizer.keyword() == STATIC || myTokenizer.keyword() == FIELD) //STATIC || FIELD
            CompileClassVarDec();
        if (myTokenizer.keyword() == CONSTRUCTOR || myTokenizer.keyword() == FUNCTION || myTokenizer.keyword() == METHOD) //CONSTRUCTOR || FUNCTION || METHOD
            CompileSubroutine();
    }
}
void CompilationEngine::CompileClassVarDec()
{
    //STATIC || FIELD
    Range r;
    Keyword k = CompileKeyword();
    if ( k == STATIC) r = STATIC_R;
    else if (k == FIELD) r = FIELD_R;
    
 
    //type
    string type;
    type = CompileType();

    //varName  
    string varName;
    do
    {
        if (isComma())
        {
            CompileSymbol();
        }
        if (myTokenizer.tokenType() == IDENTIFIER)
        {
            varName = CompileIdentifier();
            mySymbolTable.Define(varName, type, r);
        }
        else; //error
    } while (!isSemiCollon());
    CompileSymbol();
}
void CompilationEngine::CompileSubroutine()
{
    mySymbolTable.startSubroutine();
    string subroutineName = className;
    string returnType;
    subroutineName.append(".");
    CompileKeyword();
    // VOID || TYPE
    if (myTokenizer.tokenType() == KEYWORD && myTokenizer.keyword() == VOID)
    {
        returnType = "void";
        advance();
    }
    else
    {
        returnType = CompileType();
    }
    //SubroutineName
    if (myTokenizer.tokenType() == IDENTIFIER)
    {
        subroutineName.append(CompileIdentifier());
    }
    else;//error

    //(parameterlist)
    if (isParentheseStart())
        CompileSymbol();

    if (!isParentheseEnd())
        CompileParameterList();

    CompileSymbol();

    // subroutine body
    if (!isBraceStart())throw("Error:Subroutine must have body.");
    //{
    CompileSymbol();
    while (myTokenizer.tokenType() == KEYWORD && myTokenizer.keyword() == VAR)
    {
        //VarDec
        CompileVarDec();
    }
    myVMWriter.writeFunction(subroutineName, mySymbolTable.VarCount(VAR_R));
    //statements
    CompileStatements();
    // }
    CompileSymbol();
}
void CompilationEngine::CompileParameterList()
{
    if (!isType()) throw("Error: Syntax Error");

    string type = CompileType();
    string parameter = CompileIdentifier();
    mySymbolTable.Define(parameter, type, ARG_R);
    while (isComma())
    {
        CompileSymbol();
        type = CompileType();
        parameter = CompileIdentifier();
        mySymbolTable.Define(parameter, type, ARG_R);
    }
}
void CompilationEngine::CompileVarDec()
{
    //var
    CompileKeyword();
    //type
    string type = CompileType();
    //varName
    string varName = CompileIdentifier();

    mySymbolTable.Define(varName, type, VAR_R);
    while (isComma())
    {
        CompileSymbol(); //,
        varName = CompileIdentifier();  //varName
        mySymbolTable.Define(varName, type, VAR_R);
    }
    CompileSymbol();  //;
}
void CompilationEngine::CompileStatements()
{
  
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

    
}
void CompilationEngine::CompileDo()
{
    if (!isStatement()) throw("Error: Statement Error");
    //do
    CompileKeyword();
    //subroutineCall
    CompileSubroutineCall();

    if (!isSemiCollon()) throw("Error: Syntax Error");
    CompileSymbol();
}
void CompilationEngine::CompileLet()
{
    if (!isStatement()) throw("Error: Statement Error");
    //let
    CompileKeyword();
    //varName
    string varName = CompileIdentifier();
    if (isBracketStart())
    {
        //[
        CompileSymbol();
        //expression
        CompileExpression();
        if (!isBracketEnd()) throw("Error: Syntax Error");
        //]
        CompileSymbol();
    }
    //=
    CompileSymbol();
    //expression
    CompileExpression();

    if (!isSemiCollon()) throw("Error: Syntax Error");
    //;
    CompileSymbol();
    //write varName(+expression1) = expression2 
}
void CompilationEngine::CompileWhile()
{
    if (!isStatement()) throw("Error: Statement Error");
    //while
    CompileKeyword();
    myVMWriter.writeLabel("L1");
    if (!isParentheseStart())throw("Error: Syntax Error");
    //(
    CompileSymbol();
    // condition
    CompileExpression();
    if (!isParentheseEnd())throw("Error: Syntax Error");
    //)
    CompileSymbol();
    myVMWriter.writeIf("L2");

    if (!isBraceStart())throw("Error: Syntax Error");
    //{
    CompileSymbol();
    //statements
    CompileStatements();
    if (!isBraceEnd())throw("Error: Syntax Error");
    //}
    CompileSymbol();
    myVMWriter.writeGoto("L1");
    myVMWriter.writeLabel("L2");

}
void CompilationEngine::CompileReturn()
{
    if (!isStatement()) throw("Error: Statement Error");

    CompileKeyword();

    if (isExpression())
        CompileExpression();

    if (!isSemiCollon()) throw("Error: Syntax Error");
    CompileSymbol();

}
void CompilationEngine::CompileIf()
{
    if (!isStatement()) throw("Error: Statement Error");

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

}
void CompilationEngine::CompileExpression()
{
    if (!isExpression()) throw("Error: Expression Error");
    CompileTerm();

    while (isOperator())
    {
        CompileSymbol();
        CompileTerm();
    }

}
void CompilationEngine::CompileTerm()
{
    if (!isTerm()) throw("Error: Term Error");

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
}
void CompilationEngine::CompileExpressionList()
{
    if (!isExpression())
        return;
    
    CompileExpression();
    while (isComma())
    {
        CompileSymbol();
        CompileExpression();
    }
}

string CompilationEngine::CompileType()
{
    if (!isType()) throw("Error: Syntax Error");
    string s;
    if (myTokenizer.tokenType() == KEYWORD)
    {
        if (myTokenizer.keyword() == INT)
            s = "int";
        else if (myTokenizer.keyword() == CHAR)
            s ="char";
        else if (myTokenizer.keyword() == BOOLEAN)
            s = "boolean";
    }
    else if (myTokenizer.tokenType() == IDENTIFIER)
        s = myTokenizer.identifier();
    else; // error
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
    return s;
}
string CompilationEngine::CompileSubroutineCall()
{
    CompileIdentifier();
    CompileSubroutineCallwithoutName();
}
string CompilationEngine::CompileSubroutineCallwithoutName()
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
Keyword CompilationEngine::CompileKeyword() {
    
    if (myTokenizer.tokenType() != KEYWORD) throw ("Error: Syntax Error: Keyword.");
    Keyword k = myTokenizer.keyword();
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
    return k;
}
string CompilationEngine::CompileIdentifier() {
    if (myTokenizer.tokenType() != IDENTIFIER) throw ("Error: Syntax Error: Identifier.");
    string s = myTokenizer.identifier();
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
    return s;
}
string CompilationEngine::CompileSymbol() {
    if (myTokenizer.tokenType() != SYMBOL) throw ("Error: Syntax Error: Symbol.");
    string s = "";
    s += (myTokenizer.symbol());
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
    return s;
}
string CompilationEngine::CompileIntVal() {
    if (myTokenizer.tokenType() != INT_CONST) throw ("Error: Syntax Error: Integer.");
    string s = to_string(myTokenizer.intVal());
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
    return s;
}
string CompilationEngine::CompileStringVal() {
    if (myTokenizer.tokenType() != STRING_CONST) throw ("Error: Syntax Error: String.");
    string s = (myTokenizer.stringVal());
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
    return s;
}



bool CompilationEngine::isExpression() {
    return isTerm();
}
bool CompilationEngine::isTerm() {
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
bool CompilationEngine::isKeywordConstant() {
    return ((myTokenizer.tokenType() == KEYWORD) &&
        (myTokenizer.keyword() == TRUE ||
            myTokenizer.keyword() == FALSE ||
            myTokenizer.keyword() == JACK_NULL ||
            myTokenizer.keyword() == THIS));
}
bool CompilationEngine::isUnaryOperator() {
    return ((myTokenizer.tokenType() == SYMBOL) &&
        (myTokenizer.symbol() == '-' || myTokenizer.symbol() == '~'));
}
bool CompilationEngine::isOperator() {
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
bool CompilationEngine::isSubroutineCall() {
    return (myTokenizer.tokenType() == IDENTIFIER);
}
bool CompilationEngine::isParentheseStart() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == '(');
}
bool CompilationEngine::isParentheseEnd() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == ')');
}
bool CompilationEngine::isBraceStart() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == '{');
}
bool CompilationEngine::isBraceEnd() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == '}');
}
bool CompilationEngine::isBracketStart() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == '[');
}
bool CompilationEngine::isBracketEnd() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == ']');
}
bool CompilationEngine::isComma() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == ',');
}
bool CompilationEngine::isDot()
{
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == '.');
}
bool CompilationEngine::isSemiCollon() {
    return (myTokenizer.tokenType() == SYMBOL && myTokenizer.symbol() == ';');
}
bool CompilationEngine::isStatement()
{
    return((myTokenizer.tokenType() == KEYWORD) &&
        (myTokenizer.keyword() == LET ||
            myTokenizer.keyword() == IF ||
            myTokenizer.keyword() == WHILE ||
            myTokenizer.keyword() == DO ||
            myTokenizer.keyword() == RETURN)
        );
}
bool CompilationEngine::isType() {
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
void CompilationEngine::advance()
{
    myTokenizer.advance();
}