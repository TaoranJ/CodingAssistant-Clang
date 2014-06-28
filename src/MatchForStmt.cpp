#include "../include/MatchForStmt.h" 

namespace ProgrammingLanguage {

    //match for-range statement 
    StatementMatcher forRangeStmtMatcherPL = forRangeStmt().bind("forRangeStmt");
    //match for statement
    StatementMatcher forStmtMatcherPL = forStmt().bind("forStmt");
}

void ProgrammingLanguage::ForRangeStmtPrinter::run(const MatchFinder::MatchResult &Result) 
{
    clang::ASTContext *Context = Result.Context;
    const clang::CXXForRangeStmt *forRange = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("forRangeStmt");
    const clang::ForStmt *forStmt = Result.Nodes.getNodeAs<clang::ForStmt>("forStmt");

    if(forRange && !ASTUtility::IsStmtInSTDFile(forRange, Context))
        ASTUtility::Print(forRange, Context, "Rule026.ForRange");
    else if (forStmt && !ASTUtility::IsStmtInSTDFile(forStmt, Context))
        ASTUtility::Print(forStmt, Context, "Rule026.For");
}
