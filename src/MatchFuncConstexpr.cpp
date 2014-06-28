#include "../include/MatchFuncConstexpr.h"

namespace ProgrammingLanguage {

    StatementMatcher normalFuncMatcherPL = compoundStmt(
            hasParent(functionDecl().bind("function")),
            statementCountIs(1),
            has(returnStmt())
            ).bind("functionBody");

}

void ProgrammingLanguage::FunctionPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CompoundStmt *funcBody = Result.Nodes.getNodeAs<clang::CompoundStmt>("functionBody");
    const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("function");

    if(!funcBody || !func || ASTUtility::IsStmtInSTDFile(funcBody, Context)) return;

    if (func -> isConstexpr())
        ASTUtility::Print(func, Context, "Rule036.constexpr");
    else
        ASTUtility::Print(func, Context, "Rule036");
}
