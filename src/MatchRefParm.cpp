#include "../include/MatchRefParm.h" 

namespace ProgrammingLanguage {

    //match a simple funcion including method
    DeclarationMatcher funcMatcherPL = functionDecl().bind("func");

    int countRef = 0;
}

void ProgrammingLanguage::ParmPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("func");

    if(!func || !func -> hasBody() || ASTUtility::IsDeclInSTDFile(func, Context)) return;
    
    for(auto p : func -> parameters()) 
    {
        if (p -> getType() -> isReferenceType())
        {
            countRef++;
            std::ofstream out("Rule018.back");
            if (!out) return; 
            out << countRef << "\n";
            if (p -> getType().getNonReferenceType().isConstQualified()) continue;
            ASTUtility::Print(p, Context, "Rule018");
        }
    }
}
