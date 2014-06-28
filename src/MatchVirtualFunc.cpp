#include "../include/MatchVirtualFunc.h"

namespace CodStd101 {
    DeclarationMatcher pubVtlFuncMatcherCS = methodDecl(isPublic(), isVirtual()).bind("pubVtl");
}

void CodStd101::PubVtlPrinter::run(const MatchFinder::MatchResult &Result)
{
	clang::ASTContext *Context = Result.Context;
	const clang::CXXMethodDecl *cxxmd = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("pubVtl");

	if(!cxxmd || ASTUtility::IsDeclInSTDFile(cxxmd, Context)) return;

	ASTUtility::Print(cxxmd, Context, "Rule062");
}
