#include "../include/MatchUninitVar.h" 

namespace ProgrammingLanguage {
    //uninitialied var
    DeclarationMatcher uninitializedVarMatcherPL = varDecl(unless(hasInitializer(expr()))).bind("var");
}

void ProgrammingLanguage::UninitVarPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::VarDecl *var = Result.Nodes.getNodeAs<clang::VarDecl>("var");

    if(!var || ASTUtility::IsDeclInSTDFile(var, Context)) return;
    
    ASTUtility::Print(var, Context, "Rule011");
}
