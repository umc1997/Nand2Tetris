#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(const string& inputFileName, const string& className)
    :inputFileName(inputFileName.c_str()),
    className(className),
    myTokenizer(inputFileName.c_str()),
    mySymbolTable(),
    myVMWriter(inputFileName),
    IF_LABEL_NUMBER(0),
    WHILE_LABEL_NUMBER(0)
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
    CompileKeyword();
    if (myTokenizer.identifier() != className) throw("Error: File name doesn't match class name");
    mySymbolTable.setCurrentClassName(CompileIdentifier());
    if (!isBraceStart()) throw("Error: Syntax Error");
    CompileSymbol();
    while (!isBraceEnd()) {
        if (myTokenizer.keyword() == STATIC || myTokenizer.keyword() == FIELD) //STATIC || FIELD
            CompileClassVarDec();
        if (myTokenizer.keyword() == CONSTRUCTOR || myTokenizer.keyword() == FUNCTION || myTokenizer.keyword() == METHOD) //CONSTRUCTOR || FUNCTION || METHOD
            CompileSubroutine();
    }
    CompileSymbol();
}
void CompilationEngine::CompileClassVarDec()
{
    //STATIC || FIELD
    Range r = NONE_R;
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
    bool isConstructor = (myTokenizer.keyword() == CONSTRUCTOR);
    bool isMethod = (myTokenizer.keyword() == METHOD);
    mySymbolTable.startSubroutine();
    WHILE_LABEL_NUMBER = 0;
    IF_LABEL_NUMBER = 0;
    if (isMethod)
    {
        mySymbolTable.Define("this", className, ARG_R);
    }
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
    if (isConstructor)
    {
        myVMWriter.writePush("constant", mySymbolTable.VarCount(FIELD_R));
        myVMWriter.writeCall("Memory.alloc", 1);
        myVMWriter.writePop("pointer", 0);
    }
    if (isMethod)
    {
        myVMWriter.writePush(ARG_R, 0);
        myVMWriter.writePop("pointer", 0);
    }
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
    myVMWriter.writePop("temp", 0);
}
void CompilationEngine::CompileLet()
{
    if (!isStatement()) throw("Error: Statement Error");
    bool isArray = false;
    //let
    CompileKeyword();
    //varName
    string varName = CompileIdentifier();
    if (isBracketStart())
    {
        isArray = true;
        //[
        CompileSymbol();
        //expression
        CompileExpression();
        if (!isBracketEnd()) throw("Error: Syntax Error");
        //]
        CompileSymbol();
        myVMWriter.writePush(mySymbolTable.KindOf(varName), mySymbolTable.IndexOf(varName));
        myVMWriter.writerArithmetic("add");
    }
    //=
    CompileSymbol();
    //expression  TODO
    CompileExpression();

    if (!isSemiCollon()) throw("Error: Syntax Error");
    //;
    CompileSymbol();
    //write varName(+expression1) = expression2 
    if (isArray)
    {
        myVMWriter.writePop("temp", 0);
        myVMWriter.writePop("pointer", 1);
        myVMWriter.writePush("temp", 0);
        myVMWriter.writePop("that", 0);
    }
    else
    {
        myVMWriter.writePop(mySymbolTable.KindOf(varName), mySymbolTable.IndexOf(varName));
    }
}
void CompilationEngine::CompileWhile()
{
    if (!isStatement()) throw("Error: Statement Error");
    //while
    CompileKeyword();
    string L1 = "WHILE_EXP";
    string L2 = "WHILE_END";
    L1.append(to_string(WHILE_LABEL_NUMBER));
    L2.append(to_string(WHILE_LABEL_NUMBER));
    WHILE_LABEL_NUMBER++;
    myVMWriter.writeLabel(L1);
    if (!isParentheseStart())throw("Error: Syntax Error");
    //(
    CompileSymbol();
    // condition
    CompileExpression();
    if (!isParentheseEnd())throw("Error: Syntax Error");
    //)
    CompileSymbol();
    myVMWriter.writerArithmetic("not");
    myVMWriter.writeIf(L2);

    if (!isBraceStart())throw("Error: Syntax Error");
    //{
    CompileSymbol();
    //statements
    CompileStatements();
    if (!isBraceEnd())throw("Error: Syntax Error");
    //}
    CompileSymbol();
    myVMWriter.writeGoto(L1);
    myVMWriter.writeLabel(L2);

}
void CompilationEngine::CompileReturn()
{
    if (!isStatement()) throw("Error: Statement Error");
    //return
    CompileKeyword();

    if (isExpression())
        CompileExpression();
    else 
        myVMWriter.writePush("constant", 0);

    if (!isSemiCollon()) throw("Error: Syntax Error");
    //;
    CompileSymbol();
    myVMWriter.writeReturn();

}
void CompilationEngine::CompileIf()
{
    if (!isStatement()) throw("Error: Statement Error");

    //if
    CompileKeyword();

    if (!isParentheseStart())throw("Error: Syntax Error");
    //(
    CompileSymbol();
    //cond
    CompileExpression();
    if (!isParentheseEnd())throw("Error: Syntax Error");
    //)
    CompileSymbol();

    string L1 = "IF_TRUE";
    string L2 = "IF_FALSE";
    string L3 = "IF_END";
    L1.append(to_string(IF_LABEL_NUMBER));
    L2.append(to_string(IF_LABEL_NUMBER));
    L3.append(to_string(IF_LABEL_NUMBER));
    IF_LABEL_NUMBER++;

    myVMWriter.writeIf(L1);
    myVMWriter.writeGoto(L2);
    myVMWriter.writeLabel(L1);

    if (!isBraceStart())throw("Error: Syntax Error");
    //{
    CompileSymbol();
    //statements
    CompileStatements();
    if (!isBraceEnd())throw("Error: Syntax Error");
    //}
    CompileSymbol();
    if (myTokenizer.tokenType() == KEYWORD && myTokenizer.keyword() == ELSE)
    {
        myVMWriter.writeGoto(L3);
        myVMWriter.writeLabel(L2);
        CompileKeyword();
        if (!isBraceStart())throw("Error: Syntax Error");
        CompileSymbol();
        CompileStatements();
        if (!isBraceEnd())throw("Error: Syntax Error");
        CompileSymbol();
        myVMWriter.writeLabel(L3);
    }
    else
        myVMWriter.writeLabel(L2);
}
void CompilationEngine::CompileExpression()
{
    if (!isExpression()) throw("Error: Expression Error");
    CompileTerm();

    string symbol;
    while (isOperator())
    {
        symbol = CompileSymbol();
        CompileTerm();
        if (symbol == "+")
            myVMWriter.writerArithmetic("add");
        else if (symbol == "-")
            myVMWriter.writerArithmetic("sub");
        else if (symbol == "*")
            myVMWriter.writeCall("Math.multiply", 2);
        else if (symbol == "/")
            myVMWriter.writeCall("Math.divide", 2);
        else if (symbol == "&")
            myVMWriter.writerArithmetic("and");
        else if (symbol == "|")
            myVMWriter.writerArithmetic("or");
        else if (symbol == "<")
            myVMWriter.writerArithmetic("lt");
        else if (symbol == ">")
            myVMWriter.writerArithmetic("gt");
        else if (symbol == "=")
            myVMWriter.writerArithmetic("eq");
    }

}
void CompilationEngine::CompileTerm()
{
    if (!isTerm()) throw("Error: Term Error");

    string varName = "";
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
        varName = CompileIdentifier();
        //varName[]
        if (isBracketStart())
        {
            CompileSymbol();
            CompileExpression();
            if (!isBracketEnd()) throw("Error: Syntax Error");
            CompileSymbol();
            myVMWriter.writePush(mySymbolTable.KindOf(varName), mySymbolTable.IndexOf(varName));
            myVMWriter.writerArithmetic("add");
            myVMWriter.writePop("pointer", 1);
            myVMWriter.writePush("that", 0);
        }
        //subroutineCall
        else if (isParentheseStart() || isDot())
        {
            CompileSubroutineCallwithoutName(varName);
        }
        //varName
        else
        {
            myVMWriter.writePush(mySymbolTable.KindOf(varName), mySymbolTable.IndexOf(varName));
        }
        break;
    case(KEYWORD):
        if (isKeywordConstant()) CompileKeywordConstant();
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
            string unaryOp = CompileSymbol();
            CompileTerm();
            if (unaryOp == "-")
                myVMWriter.writerArithmetic("neg");
            else if (unaryOp == "~")
                myVMWriter.writerArithmetic("not");
        }
        else; //error
        break;
    }
}
int CompilationEngine::CompileExpressionList()
{
    if (!isExpression())
        return 0;
    int numArgs = 0;
    CompileExpression();
    numArgs++;
    while (isComma())
    {
        CompileSymbol();
        CompileExpression();
        numArgs++;
    }
    return numArgs;
}
void  CompilationEngine::CompileKeywordConstant()
{
    switch (myTokenizer.keyword())
    {
    case(TRUE):
        myVMWriter.writePush("constant", 0);
        myVMWriter.writerArithmetic("not");
        break;
    case(FALSE):
    case(JACK_NULL):
        myVMWriter.writePush("constant", 0);
        break;
    case(THIS):
        myVMWriter.writePush("pointer", 0);
        break;
    default:
        break;
    }
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
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
void CompilationEngine::CompileSubroutineCall()
{
    string name = CompileIdentifier();
    CompileSubroutineCallwithoutName(name);
}
void CompilationEngine::CompileSubroutineCallwithoutName(const string& name)
{
    if (!isParentheseStart() && !isDot()) throw("Error: Syntax Error");
    string subroutineName = className;
    int numArgs = 0;
    if (isParentheseStart())
    {
        subroutineName.append(".");
        subroutineName.append(name);
        numArgs++;
        CompileSymbol();
        myVMWriter.writePush("pointer", 0);
        numArgs += CompileExpressionList();
        if (!isParentheseEnd()) throw("Error: Syntax Error");
        CompileSymbol();  
    }
    else if (isDot())
    {
        if (mySymbolTable.isExist(name))
        {
            myVMWriter.writePush(mySymbolTable.KindOf(name), mySymbolTable.IndexOf(name));
            subroutineName = mySymbolTable.TypeOf(name);
            numArgs++;
        }
        else {
            subroutineName = name;
        }
        CompileSymbol();
        string s = CompileIdentifier();
        subroutineName.append(".");
        subroutineName.append(s);
        if (!isParentheseStart())throw("Error: Syntax Error");
        CompileSymbol();
        numArgs += CompileExpressionList();
        if (!isParentheseEnd()) throw("Error: Syntax Error");
        CompileSymbol();
    }
    myVMWriter.writeCall(subroutineName, numArgs);
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
    myVMWriter.writePush("constant", myTokenizer.intVal());
    if (myTokenizer.hasMoreTokens()) myTokenizer.advance();
    return s;
}
string CompilationEngine::CompileStringVal() {
    if (myTokenizer.tokenType() != STRING_CONST) throw ("Error: Syntax Error: String.");
    string s = (myTokenizer.stringVal());
    int length = myTokenizer.stringVal().length();
    myVMWriter.writePush("constant", length);
    myVMWriter.writeCall("String.new", 1);
    for (int i = 0; i < length; i++)
    {
        int ascii = myTokenizer.stringVal()[i];
        myVMWriter.writePush("constant" , ascii);
        myVMWriter.writeCall("String.appendChar", 2);
    }
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