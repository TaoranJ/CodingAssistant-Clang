#include "../include/MatchCast.h"

namespace ProgrammingLanguage {

    StatementMatcher explicitCastMatcherPL = explicitCastExpr().bind("explicitcastexpr");
}

void ProgrammingLanguage::ExplicitCastPrinter::run(const MatchFinder::MatchResult &Result)
{
    //get the ASTContext
    clang::ASTContext *Context = Result.Context;
    const clang::ExplicitCastExpr *ece = Result.Nodes.getNodeAs<clang::ExplicitCastExpr>("explicitcastexpr");
    
    if(!ece || ASTUtility::IsStmtInSTDFile(ece, Context)) return;
    
    if (clang::CXXReinterpretCastExpr::classof(ece))
        ASTUtility::Print(ece, Context, "Rule045");
    else if (clang::CXXStaticCastExpr::classof(ece))
        ASTUtility::Print(ece, Context, "Rule046");
    else if (clang::CXXConstCastExpr::classof(ece))
        ASTUtility::Print(ece, Context, "Rule047");
    else if (clang::CStyleCastExpr::classof(ece))
        ASTUtility::Print(ece, Context, "Rule048");
}
