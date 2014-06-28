#include "../include/MatchArray.h" 

namespace ProgrammingLanguage {
    //match built-in array[]
    DeclarationMatcher arrayMatcherPL = declaratorDecl(hasType(arrayType())).bind("var");
}

void ProgrammingLanguage::ArrayPrinter::run(const MatchFinder::MatchResult &Result) 
{
    clang::ASTContext* Context = Result.Context;
    //get the node
    const clang::DeclaratorDecl *var = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("var");

    //if not valid, return
    if(!var || ASTUtility::IsDeclInSTDFile(var, Context)) return;

    //invalid means user-defined type, when we can't find right include
    //files.
    if (var -> isInvalidDecl())
        ASTUtility::Print(var, Context, "Rule003");
    else
    {
        if(var -> getTypeSourceInfo() -> getType() -> getBaseElementTypeUnsafe() -> isBuiltinType())
            ASTUtility::Print(var, Context, "Rule002");
        else
            ASTUtility::Print(var, Context, "Rule003");
    }
}
