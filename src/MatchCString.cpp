#include "../include/MatchCString.h"

namespace ProgrammingLanguage {
    DeclarationMatcher charPointerMatcherPL = declaratorDecl(hasType(pointsTo(
                    builtinType().bind("pType")))).bind("pVar");
    
    DeclarationMatcher charArrayMatcherPL = declaratorDecl(hasType(arrayType(
                    hasElementType(builtinType().bind("aType"))))).bind("aVar");
    
    DeclarationMatcher arrayOfCharPointerMatcherPL = declaratorDecl(hasType(arrayType(
                        hasElementType(pointerType(pointee(builtinType().bind("apType"))))))
            ).bind("apVar");
    
    DeclarationMatcher charPtrPtrMatcherPL = declaratorDecl(hasType(pointsTo(
                        pointerType(pointee(builtinType().bind("ppType")))))
            ).bind("ppVar");
}

void ProgrammingLanguage::CharPointerPrinter::run(const MatchFinder::MatchResult &Result)
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    //charPointerMatcher
    const clang::BuiltinType *pType = Result.Nodes.getNodeAs<clang::BuiltinType>("pType");
    const clang::DeclaratorDecl *pVar = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("pVar");
    //charArrayMatcher
    const clang::BuiltinType *aType = Result.Nodes.getNodeAs<clang::BuiltinType>("aType");
    const clang::DeclaratorDecl *aVar = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("aVar");
    //arrayOfCharPointerMatcher
    const clang::BuiltinType *apType = Result.Nodes.getNodeAs<clang::BuiltinType>("apType");
    const clang::DeclaratorDecl *apVar = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("apVar");
    //charPtrPtrMatcher
    const clang::BuiltinType *ppType = Result.Nodes.getNodeAs<clang::BuiltinType>("ppType");
    const clang::DeclaratorDecl *ppVar = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("ppVar");
    
    
    if (pType && pVar && !ASTUtility::IsDeclInSTDFile(pVar, Context) && pType -> isAnyCharacterType())
        ASTUtility::Print(pVar, Context, "Rule001");
    else if (aType && aVar && !ASTUtility::IsDeclInSTDFile(aVar, Context) && aType -> isAnyCharacterType())
        ASTUtility::Print(aVar, Context, "Rule001");
    else if (apType && apVar && !ASTUtility::IsDeclInSTDFile(apVar, Context) && apType -> isAnyCharacterType())
        ASTUtility::Print(apVar, Context, "Rule001");
    else if (ppType && ppVar && !ASTUtility::IsDeclInSTDFile(ppVar, Context) && ppType -> isAnyCharacterType())
        ASTUtility::Print(ppVar, Context, "Rule001");
}
