#include "../include/MatchRealloc.h"

namespace ProgrammingLanguage {

    //find func realloc 
    StatementMatcher callExprMatcherPL = callExpr(callee(
                functionDecl(hasName("realloc")))).bind("callFun");
}

void ProgrammingLanguage::CallExprPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CallExpr *callFun = Result.Nodes.getNodeAs<clang::CallExpr>("callFun");

    if(!callFun || ASTUtility::IsStmtInSTDFile(callFun, Context)) return;

    ASTUtility::Print(callFun, Context, "Rule007");        
}
