#include "../include/MatchAryAsPtr.h"

namespace ProgrammingLanguage {

    StatementMatcher simpleImpCastMatcherPL = callExpr(
            findAll(implicitCastExpr().bind("imp"))).bind("expr");
}

void ProgrammingLanguage::ArrayAsPtrPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::ImplicitCastExpr *impCast = Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("imp");
    const clang::CallExpr *call = Result.Nodes.getNodeAs<clang::CallExpr>("expr");

    if(!impCast || ASTUtility::IsStmtInSTDFile(call, Context)) return;

    std::string str = "ArrayToPointerDecay";
    if( str == impCast -> getCastKindName() )
        ASTUtility::Print(call, Context, "Rule040");
}
