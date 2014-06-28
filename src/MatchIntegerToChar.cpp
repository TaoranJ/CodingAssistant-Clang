#include "../include/MatchIntegerToChar.h" 

namespace ProgrammingLanguage {
    //match  {int x} = 'c';
    DeclarationMatcher charLiteralToIntMatcherPL = varDecl(hasInitializer(implicitCastExpr(
                    hasDescendant(characterLiteral()),
                    hasType(isInteger())))).bind("varCharL");
    //match {char} c = 12;
    DeclarationMatcher intLiteralToCharMatcherPL = varDecl(hasInitializer(implicitCastExpr(
                    hasDescendant(integerLiteral())))).bind("varIntL");
    //match int x = 1; char c = x;
    DeclarationMatcher intVarToCharMatcherPL = varDecl(hasInitializer(implicitCastExpr(
                    hasDescendant(declRefExpr(hasType(isInteger())).bind("intRef")),
                    hasType(isInteger())))).bind("varChar");
    //match i = c; or c = i
    StatementMatcher equOpMatcherPL = binaryOperator(hasLHS(declRefExpr().bind("declRefL")),
                    hasRHS(implicitCastExpr(hasDescendant(declRefExpr().bind("declRefR")))),
                    hasOperatorName("=")
                    ).bind("bo");
}

void ProgrammingLanguage::CharIntPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::VarDecl *varCharL = Result.Nodes.getNodeAs<clang::VarDecl>("varCharL");
    const clang::VarDecl *varIntL = Result.Nodes.getNodeAs<clang::VarDecl>("varIntL");
    const clang::VarDecl *varChar = Result.Nodes.getNodeAs<clang::VarDecl>("varChar");
    const clang::DeclRefExpr *intRef = Result.Nodes.getNodeAs<clang::DeclRefExpr>("intRef");
    const clang::DeclRefExpr *declRefL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("declRefL");
    const clang::DeclRefExpr *declRefR = Result.Nodes.getNodeAs<clang::DeclRefExpr>("declRefR");
    const clang::BinaryOperator *equOp = Result.Nodes.getNodeAs<clang::BinaryOperator>("bo");

    //situation 1: {int} = 'c'; remove signed/unsigned char = 'c'
    if(varCharL && !ASTUtility::IsDeclInSTDFile(varCharL, Context)) 
    {
        //for charLiteralToIntMatcher, sth like signed char = 'c' will also
        //invoke this method.
        if (varCharL -> getType() -> isCharType()) return;
        ASTUtility::Print(varCharL, Context, "Rule015");
    }

    //situation 2: {char} = 12;
    if (varIntL && !ASTUtility::IsDeclInSTDFile(varIntL, Context))
    {
        if (!varIntL -> getType() -> isCharType()) return;
        ASTUtility::Print(varIntL, Context, "Rule015");
    }

    //situation 3: {char} c = {int}x;
    //situation 4: {int} c = {char}x;
    if (varChar && intRef && !ASTUtility::IsDeclInSTDFile(varChar, Context))
    {
        //left is a char and right is a int for situation 3
        //left is a integer and right is a char for situation 4
        if ((intRef -> getType() -> isIntegerType() && varChar -> getType() -> isCharType()) ||  
                (intRef -> getType() -> isCharType() && varChar -> getType() -> isIntegerType()))
            ASTUtility::Print(varChar, Context, "Rule015");
    }       

    //situation 5: c = i; i = c
    if (equOp && declRefL && declRefR && !ASTUtility::IsStmtInSTDFile(declRefL, Context) && !ASTUtility::IsStmtInSTDFile(declRefR, Context))
    {
        //left is a char and right is not a integer. c = i
        if ((declRefL -> getType() -> isCharType() && declRefR -> getType() -> isIntegerType()) ||
                (declRefL -> getType() -> isIntegerType() && declRefR -> getType() -> isCharType()))
            ASTUtility::Print(equOp, Context, "Rule015");
    }
}
