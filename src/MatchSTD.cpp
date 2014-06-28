#include "../include/MatchSTD.h" 

namespace ProgrammingLanguage {

    //match function here
    StatementMatcher funcCallMatcherPL = callExpr(callee(functionDecl().bind("funcDecl"))).bind("callExpr");
    //match var declarations here
    DeclarationMatcher varMatcherPL = varDecl().bind("var");
}

void ProgrammingLanguage::STDPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CallExpr *call = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr");
    const clang::FunctionDecl *fun = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl");
    const clang::VarDecl *var = Result.Nodes.getNodeAs<clang::VarDecl>("var");

    if(call && !ASTUtility::IsStmtInSTDFile(call, Context))
    {
        if (ASTUtility::IsDeclInSTDFile(fun, Context)) 
            ASTUtility::Print(fun, Context, "Rule029.stmt.std");
        else
            ASTUtility::Print(fun, Context, "Rule029.stmt.usr");
    }

    if (var && !ASTUtility::IsDeclInSTDFile(var, Context))
    {
        if (var -> getType().getAsString().find("std::") != std::string::npos)
            ASTUtility::Print(var, Context, "Rule029.decl.std");
        else
            ASTUtility::Print(var, Context, "Rule029.decl.usr");
    }
}
