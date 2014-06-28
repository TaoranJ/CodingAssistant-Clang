#include "../include/MatchNotPlainChar.h"

namespace ProgrammingLanguage {

    //match only normal declaration 
    DeclarationMatcher notPlainCharMatcherPL = declaratorDecl().bind("var");
}

void ProgrammingLanguage::NotPlainCharPrinter::run(const MatchFinder::MatchResult &Result) 
{
    clang::ASTContext *Context = Result.Context;
    const clang::DeclaratorDecl *var = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("var");

    if(!var || ASTUtility::IsDeclInSTDFile(var, Context)) return;
    
    //function
    if( clang::FunctionDecl::classof(var) )
    {
        std::string returnType = static_cast<const clang::FunctionDecl*>(var) -> getReturnType().getAsString();
        if (returnType.find("unsigned char") != std::string::npos || returnType.find("signed char") != std::string::npos) 
            ASTUtility::Print(var, Context, "Rule008");
    }
    else
    {
        std::string type = var -> getType().getAsString();
        if (type.find("signed char") != std::string::npos || type.find("unsigned char") != std::string::npos) 
            ASTUtility::Print(var, Context, "Rule008");
    }
}
