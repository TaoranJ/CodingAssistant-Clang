#include "../include/MatchBitField.h" 

namespace ProgrammingLanguage {

    //match sturct/union/class 
    DeclarationMatcher recordFieldDeclMatcherPL = recordDecl(has(fieldDecl())).bind("recordDecl");
}

void ProgrammingLanguage::BitFieldPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::RecordDecl *st = Result.Nodes.getNodeAs<clang::RecordDecl>("recordDecl");
    std::vector<std::string> typeList;

    //only need struct
    if(!st || !st -> isCompleteDefinition() || !st -> isStruct() || ASTUtility::IsDeclInSTDFile(st, Context)) return;

    for(auto it = st -> field_begin(); it != st -> field_end(); it++)
        if((*it) -> isBitField())
            ASTUtility::Print(*it, Context, "Rule024");
}
