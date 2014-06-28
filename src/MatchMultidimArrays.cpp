#include "../include/MatchMultidimArrays.h"

namespace ProgrammingLanguage {
    
    //match type array[][]
    DeclarationMatcher multiArrayMatcherPL = declaratorDecl(hasType(arrayType(hasElementType(arrayType())))).bind("varArray");
}

void ProgrammingLanguage::MultiArrayPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::DeclaratorDecl *varArray = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("varArray");

    if(!varArray || ASTUtility::IsDeclInSTDFile(varArray, Context)) return;

    ASTUtility::Print(varArray, Context, "Rule016");
}
