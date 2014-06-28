#include "../include/MatchPassByValue.h" 

namespace ProgrammingLanguage {

    //match char
    DeclarationMatcher funcParmMatcherPL = functionDecl(
            findAll(parmVarDecl().bind("parm")),
            isDefinition()).bind("func");
}

void ProgrammingLanguage::FuncParmPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("func");
    const clang::ParmVarDecl *parm = Result.Nodes.getNodeAs<clang::ParmVarDecl>("parm");

    if(!func || !parm || ASTUtility::IsDeclInSTDFile(func, Context)) return;
    
    if (parm -> getType() -> isPointerType() || parm -> getType() -> isArrayType() ||
            parm -> getType() -> isReferenceType()) return;

    //not the parameter is passed by value
    if (!parm -> getType() -> getBaseElementTypeUnsafe() -> isBuiltinType())
        ASTUtility::Print(parm, Context, "Rule038");
}
