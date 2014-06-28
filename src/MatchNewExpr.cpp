#include "../include/MatchNewExpr.h"

namespace ProgrammingLanguage {

    StatementMatcher cxxNewExprMatcherPL = newExpr().bind("newExpr");
}

void ProgrammingLanguage::NewExprPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CXXNewExpr *newExpr = Result.Nodes.getNodeAs<clang::CXXNewExpr>("newExpr");
    
    if(!newExpr || ASTUtility::IsStmtInSTDFile(newExpr, Context)) return;

    ASTUtility::Print(newExpr, Context, "Rule031");
}
