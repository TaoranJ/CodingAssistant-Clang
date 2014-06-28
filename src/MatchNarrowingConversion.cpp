#include "../include/MatchNarrowingConversion.h"

namespace ProgrammingLanguage {
    StatementMatcher norCastMatcherPL = implicitCastExpr().bind("impCast");
}

void ProgrammingLanguage::NarrowCastPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::ImplicitCastExpr *cast = Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("impCast");

    if(!cast || ASTUtility::IsStmtInSTDFile(cast, Context)) return;
    
    std::string castType(cast -> getCastKindName());
    if(castType == "DerivedToBase" || castType == "UncheckedDerivedToBase" 
       || castType == "FloatingToIntegral" || castType == "FloatingToBoolean" 
       || castType == "IntegralToBoolean" || castType ==  "ToVoid")
    {
        ASTUtility::Print(cast, Context, "Rule030");
    }
}
