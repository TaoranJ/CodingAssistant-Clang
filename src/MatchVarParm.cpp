#include "../include/MatchVarParm.h"

namespace ProgrammingLanguage {

    StatementMatcher funcVarParmMatcherPL = compoundStmt(
            hasParent(functionDecl().bind("function"))         
            ).bind("functionBody");
}

void ProgrammingLanguage::FunctionVarParmPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CompoundStmt *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("functionBody");
    const clang::FunctionDecl *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("function");

    if(!FD || ASTUtility::IsDeclInSTDFile(FD, Context)) return;
    
    if(FD -> isVariadic())
        ASTUtility::Print(FD, Context, "Rule041");
}
