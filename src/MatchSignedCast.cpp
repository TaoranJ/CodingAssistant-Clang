#include "../include/MatchSignedCast.h" 

namespace ProgrammingLanguage {

    //only match implicit cast
    StatementMatcher impCastExprMatcherPL = implicitCastExpr().bind("cast");
}

void ProgrammingLanguage::ImpCastExprPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::ImplicitCastExpr *cast = Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("cast");

    if(!cast || ASTUtility::IsStmtInSTDFile(cast, Context)) return;

    std::string left = cast -> getType().getAsString();
    std::string right = cast -> getSubExpr() -> getType().getAsString();

    //unsigned ... = signed... OR signed(or null) ... = unsigned ...
    if ((left.find("unsigned ") != std::string::npos && right.find("unsigned ") == std::string::npos) ||
            (left.find("unsigned ") == std::string::npos && right.find("unsigned ") != std::string::npos))
        ASTUtility::Print(cast, Context, "Rule009");
}
