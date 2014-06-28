#include "../include/MatchAuto.h" 

namespace ProgrammingLanguage {
    
    //only match var, if auto is tested in the AutoVarPrinter
    DeclarationMatcher autoVarMatcherPL = varDecl().bind("var");
}

void ProgrammingLanguage::AutoVarPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::VarDecl *var = Result.Nodes.getNodeAs<clang::VarDecl>("var");

    if(!var || ASTUtility::IsDeclInSTDFile(var, Context) ) return;

    //not auto, return
    if(!var -> getType() -> getContainedAutoType()) return;

    std::ofstream out("Rule012.back");
    if (!out) return;
    out << "true" << std::endl;

    //auto without =
    std::string expr = ASTUtility::Print(var, Context);

    if(expr.find("=") == std::string::npos)
        ASTUtility::Print(var, Context, "Rule012");
}
