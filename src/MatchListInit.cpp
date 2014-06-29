#include "../include/MatchListInit.h" 

namespace ProgrammingLanguage {

    //int x = {2}     or int x{2}
    DeclarationMatcher varDeclMatcherPL = varDecl(hasInitializer(initListExpr())).bind("var"); 
    //for class
    DeclarationMatcher classVarDeclMatcherPL = varDecl(hasInitializer(
                constructExpr(isListInitialization()))).bind("varClass");
}

void ProgrammingLanguage::InitListPrinter::run (const MatchFinder::MatchResult& Result)
{
    clang::ASTContext* Context = Result.Context;
    const clang::VarDecl* varNormal = Result.Nodes.getNodeAs<clang::VarDecl>("var");
    const clang::VarDecl* varClass = Result.Nodes.getNodeAs<clang::VarDecl>("varClass");

    if (varNormal && !ASTUtility::IsDeclInSTDFile(varNormal, Context))
        ASTUtility::Print(varNormal, Context, "Rule013"); 
    else if (varClass && !ASTUtility::IsDeclInSTDFile(varClass, Context))
    {
        ASTUtility::Print(varClass, Context, "Rule014"); 
        std::ofstream out("Rule014.back");
        if(!out) return;
        out << "true" << std::endl;
    }
}
