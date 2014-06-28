#include "../include/MatchReturnWarning.h"

namespace ProgrammingLanguage {

    //function
    StatementMatcher returnMatcherPL = compoundStmt(
            hasParent(functionDecl().bind("func")),
            findAll(returnStmt().bind("stmt")));
}

void ProgrammingLanguage::ReturnPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("func");
    const clang::ReturnStmt *retStmt = Result.Nodes.getNodeAs<clang::ReturnStmt>("stmt");

    if(!func || !retStmt || ASTUtility::IsDeclInSTDFile(func, Context)) return;

    //overloader operator
    if( func -> isOverloadedOperator() ) return;

    if (func -> getReturnType() -> isPointerType() || func -> getReturnType() -> isReferenceType())
        ASTUtility::Print(func, Context, "Rule039");
}
