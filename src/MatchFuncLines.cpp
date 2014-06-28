#include "../include/MatchFuncLines.h"

namespace CodStd101 {
    //threshold value , it can be changed in configure file
    const int funcThreshold = 35;
    
    StatementMatcher funcStmtMatcherCS = compoundStmt(
    				hasParent(functionDecl().bind("functiondecl"))).bind("funcstmt");
}

void CodStd101::FuncLinesPrinter::run(const MatchFinder::MatchResult &Result)
{
    //get the ASTContext
    clang::ASTContext *Context = Result.Context;
    //get the CompoundStmt Node Relate to Function
    const clang::CompoundStmt *cs = Result.Nodes.getNodeAs<clang::CompoundStmt>("funcstmt");
    const clang::FunctionDecl *fd = Result.Nodes.getNodeAs<clang::FunctionDecl>("functiondecl");
    
    if(!cs || !fd || !Context->getSourceManager().isInMainFile(cs->getLBracLoc()) || ASTUtility::IsDeclInSTDFile(fd, Context))
    	return;
    
    clang::FullSourceLoc startLoc = Context->getFullLoc(cs->getLocStart());
    clang::FullSourceLoc endLoc = Context->getFullLoc(cs->getLocEnd());

    if(startLoc.isValid() && endLoc.isValid())
    {
    	int startLine = startLoc.getSpellingLineNumber();
    	int endLine = endLoc.getSpellingLineNumber();
        if (endLine - startLine >= funcThreshold)
            ASTUtility::Print(fd, Context, "Rule050");
    }
}
