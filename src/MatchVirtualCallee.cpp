#include "../include/MatchVirtualCallee.h"

namespace EffectiveCPP {

    StatementMatcher ctorCallVtlMatcherEC = compoundStmt(
            hasParent(constructorDecl().bind("cDecl")),
            hasDescendant(callExpr(callee(methodDecl(isVirtual()))))
            );
    
    StatementMatcher dtorCallVtlMatcherEC = compoundStmt(
            hasParent(destructorDecl().bind("dDecl")),
            hasDescendant(callExpr(callee(methodDecl(isVirtual()))))
            );
}

void EffectiveCPP::CtorCallVtlPrinter::run(const MatchFinder::MatchResult &Result)
{
	clang::ASTContext *Context = Result.Context;
    const clang::CXXConstructorDecl *construct = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("cDecl");
    const clang::CXXDestructorDecl *destruct = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("dDecl");

    if (construct && !ASTUtility::IsDeclInSTDFile(construct, Context))
        ASTUtility::Print(construct, Context, "Rule055");
    if (destruct && !ASTUtility::IsDeclInSTDFile(destruct, Context))
        ASTUtility::Print(destruct, Context, "Rule055");
}
