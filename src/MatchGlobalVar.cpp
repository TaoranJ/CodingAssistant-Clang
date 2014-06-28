#include "../include/MatchGlobalVar.h"

namespace CodStd101 {
    const int globalVarLength = 7;

    //Declare a matcher of ASTNode to match var;
    DeclarationMatcher normalVarDeclMatcherCS = varDecl().bind("vardecl");
}

void CodStd101::GlobalVarPrinter::run(const MatchFinder::MatchResult &Result)
{
    //get the ASTContext
    clang::ASTContext *Context = Result.Context;
    //get the VarASTNode
    const clang::VarDecl *vd = Result.Nodes.getNodeAs<clang::VarDecl>("vardecl");
    //TODO handle the condition "without vaild data"
    if(!vd || ASTUtility::IsDeclInSTDFile(vd, Context)) return;
    
    /*
     * distinguish Global Var
     * hasGlobalStorage():distinguish GloabalVar or StaticLocalVar
     * isStaticLocal():distinguish StaticLocalVar
     */	
    if(vd->hasGlobalStorage() && (vd->isStaticLocal() == false))
    {
    	//vd is a globalVar pointer
    	std::string globalVar(vd->getNameAsString());
    	if(globalVar.size() < globalVarLength)
            ASTUtility::Print(vd, Context, "Rule051");
        ASTUtility::Print(vd, Context, "Rule052");
    }
}
