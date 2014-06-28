#include "../include/MatchHandleClass.h"

namespace ProgrammingLanguage {

    //match the the expr who calls a class constructor
    StatementMatcher classConstructMatcherPL = constructExpr(
        hasDescendant(newExpr())).bind("handle");
}

void ProgrammingLanguage::NewExprDeclPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CXXConstructExpr *construct = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("handle");

    if(!construct || ASTUtility::IsStmtInSTDFile(construct, Context)) return;
    
    //if std
    if (ASTUtility::IsDeclInSTDFile(construct -> getConstructor(), Context)) 
        ASTUtility::Print(construct, Context, "Rule019");
    else
        ASTUtility::Print(construct, Context, "Rule020");
}
