#include "../include/MatchEnum.h" 

namespace ProgrammingLanguage {

    //match sturct/union/class 
    DeclarationMatcher enumDeclMatcherPL = enumDecl().bind("enumDecl");
}

void ProgrammingLanguage::EnumDeclPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::EnumDecl *enumerator = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl");

    if(!enumerator || ASTUtility::IsDeclInSTDFile(enumerator, Context)) return; 

    if (!enumerator -> isScopedUsingClassTag())
        ASTUtility::Print(enumerator, Context, "Rule025");
}
