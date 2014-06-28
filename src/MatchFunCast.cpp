#include "../include/MatchFunCast.h"

namespace ProgrammingLanguage {

    StatementMatcher funcCastMatcherPL = functionalCastExpr().bind("funcCast");
}

void ProgrammingLanguage::FunctionalCastPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CXXFunctionalCastExpr *funCast = Result.Nodes.getNodeAs<clang::CXXFunctionalCastExpr>("funcCast");

    if(!funCast || ASTUtility::IsStmtInSTDFile(funCast, Context)) return;
    
    ASTUtility::Print(funCast, Context, "Rule034");
}
