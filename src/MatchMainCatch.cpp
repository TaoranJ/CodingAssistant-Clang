#include "../include/MatchMainCatch.h"

namespace ProgrammingLanguage {

    //find func with try catch in it
   StatementMatcher tryInFuncMatcherPL = compoundStmt(
            hasParent(functionDecl().bind("func")),         
            hasDescendant(tryStmt())).bind("funcBody");
}


void ProgrammingLanguage::TryInFuncPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CompoundStmt *funcBody = Result.Nodes.getNodeAs<clang::CompoundStmt>("funcBody");
    const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("func");

    if(!funcBody || !func || ASTUtility::IsStmtInSTDFile(funcBody, Context)) return;

    if(func -> isMain()) ASTUtility::Print(funcBody, Context, "Rule006");        
}
