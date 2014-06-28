#include "../include/MatchDoStmt.h" 

namespace ProgrammingLanguage {

    //match do statement
    StatementMatcher doStmtMatcherPL = doStmt().bind("doStmt");
}

void ProgrammingLanguage::DoStmtPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::DoStmt *doStmt = Result.Nodes.getNodeAs<clang::DoStmt>("doStmt");

    if(!doStmt || ASTUtility::IsStmtInSTDFile(doStmt, Context)) return;

    ASTUtility::Print(doStmt, Context, "Rule027");
}
