#include "../include/MatchPointerMatch.h"

namespace ProgrammingLanguage {
    StatementMatcher binaryOperatorMatcherPL = binaryOperator(
            hasLHS(ignoringImpCasts(declRefExpr(hasType(pointerType())))),
            hasRHS(ignoringImpCasts(integerLiteral()))).bind("binaryoperator");
}

void ProgrammingLanguage::PtrMemOpPrinter::run(const MatchFinder::MatchResult &Result)
{
	clang::ASTContext *Context = Result.Context;
	const clang::BinaryOperator *bop = Result.Nodes.getNodeAs<clang::BinaryOperator>("binaryoperator");
    
	if(!bop || ASTUtility::IsStmtInSTDFile(bop, Context) || bop -> isRelationalOp()) return;

	ASTUtility::Print(bop, Context, "Rule060");
}
