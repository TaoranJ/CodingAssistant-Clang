#include "../include/MatchGotoStmt.h" 

namespace ProgrammingLanguage {

    //match do statement
    StatementMatcher gotoStmtMatcherPL = gotoStmt().bind("gotoStmt");
}

void ProgrammingLanguage::GotoStmtPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::GotoStmt *gtStmt = Result.Nodes.getNodeAs<clang::GotoStmt>("gotoStmt");

    if(!gtStmt || ASTUtility::IsStmtInSTDFile(gtStmt, Context)) return;

    ASTUtility::Print(gtStmt, Context, "Rule028");
}
