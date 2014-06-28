#include "../include/MatchScope.h"

namespace ProgrammingLanguage {
    const int scopeThreshold = 10;
    //I'm not sure. Function has been checked, so no FunctionDecl
    //here find only the leaf scope in which there is no scope anymore.
    StatementMatcher scopeStmtMatcherPL = compoundStmt(
            //unless(hasParent(functionDecl())),
            unless(hasDescendant(compoundStmt()))
            ).bind("scopestmt");
}

void ProgrammingLanguage::ScopeLinesPrinter::run(const MatchFinder::MatchResult &Result)
{
	clang::ASTContext *Context = Result.Context;
	const clang::CompoundStmt *leafScope = Result.Nodes.getNodeAs<clang::CompoundStmt>("scopestmt");
    
	if(!leafScope || ASTUtility::IsStmtInSTDFile(leafScope, Context)) return;

	clang::FullSourceLoc startLoc = Context -> getFullLoc(leafScope -> getLocStart());
	clang::FullSourceLoc endLoc = Context -> getFullLoc(leafScope -> getLocEnd());
	if (!startLoc.isValid() || !endLoc.isValid()) return;
	int startLine = startLoc.getSpellingLineNumber();
	int endLine = endLoc.getSpellingLineNumber();
	int scopeLine = endLine - startLine;
    if (scopeLine > scopeThreshold)
            ASTUtility::Print(leafScope, Context, "Rule063");
}
