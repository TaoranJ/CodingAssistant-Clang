#include "../include/MatchContainer.h" 

namespace ProgrammingLanguage {
    //match overloaded []
    StatementMatcher callMatcherPL = operatorCallExpr(hasOverloadedOperatorName("[]")).bind("op");
}

void ProgrammingLanguage::CallPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CXXOperatorCallExpr *COCE = Result.Nodes.getNodeAs<clang::CXXOperatorCallExpr>("op");

    //if not null
    if(!COCE || ASTUtility::IsStmtInSTDFile(COCE, Context)) return;
    
    //if it's not a class method, or its caller has nothing to do with the std
    //container return
    if (!clang::CXXMethodDecl::classof(COCE -> getCalleeDecl())) return;

    const clang::CXXMethodDecl *CMD = static_cast<const clang::CXXMethodDecl *>(COCE -> getCalleeDecl());
    if (!ASTUtility::IsSTDContainer(CMD -> getParent())) return;

    //if the callee is in std file, without range-check 
    if (ASTUtility::IsDeclInSTDFile(COCE -> getCalleeDecl(), Context))
        ASTUtility::Print(COCE, Context, "Rule005");
    else
        ASTUtility::Print(COCE, Context, "Rule004");
}
