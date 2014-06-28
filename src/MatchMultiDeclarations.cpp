#include "../include/MatchMultiDeclarations.h"

namespace  ProgrammingLanguage {

    //match decl stmt
    StatementMatcher declStmtMatcherPL = declStmt().bind("declStmt");
}

void ProgrammingLanguage::MultiDeclPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::DeclStmt *declStmt = Result.Nodes.getNodeAs<clang::DeclStmt>("declStmt");

    if(!declStmt || ASTUtility::IsStmtInSTDFile(declStmt, Context)) return;
    
    if( !(declStmt -> isSingleDecl()) ) 
        ASTUtility::Print(declStmt, Context, "Rule010");
}
