#include "../include/MatchNakedNew.h"

namespace ProgrammingLanguage {

    //match naked new
    DeclarationMatcher nakedNewMatcherPL = varDecl(hasInitializer(newExpr())).bind("var"); 
}

void ProgrammingLanguage::NakedNewPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::VarDecl *var = Result.Nodes.getNodeAs<clang::VarDecl>("var");

    if(!var || ASTUtility::IsDeclInSTDFile(var, Context)) return; 

    ASTUtility::Print(var, Context, "Rule021");
}
