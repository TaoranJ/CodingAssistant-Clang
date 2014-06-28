#include "../include/MatchDynamicCast.h"

namespace ProgrammingLanguage {
    StatementMatcher dynamicCastMatcherPL = dynamicCastExpr().bind("dynCast");
}

void ProgrammingLanguage::DynamicCastPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CXXDynamicCastExpr *dynCast = Result.Nodes.getNodeAs<clang::CXXDynamicCastExpr>("dynCast");

    if(!dynCast && ASTUtility::IsStmtInSTDFile(dynCast, Context)) return;
    
    ASTUtility::Print(dynCast, Context, "Rule035");
}
