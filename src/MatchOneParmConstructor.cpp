#include "../include/MatchOneParmConstructor.h"

namespace ProgrammingLanguage {

    DeclarationMatcher classOneParmConstructorMatcherPL = recordDecl(
                isDefinition(),
                findAll(constructorDecl(parameterCountIs(1)).bind("oneParmConstructor"))
                ).bind("targetClass");
}

void ProgrammingLanguage::OneParmConstructorPrinter::run(const MatchFinder::MatchResult &Result)
{
    clang::ASTContext *Context = Result.Context;
    const clang::CXXRecordDecl *targetClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("targetClass");
    const clang::CXXConstructorDecl *targetConstructor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("oneParmConstructor");

    if (!targetClass || ASTUtility::IsDeclInSTDFile(targetClass, Context) ||
            !targetConstructor || ASTUtility::IsDeclInSTDFile(targetConstructor, Context))
        return;

    if (!clang::CXXConstructorDecl::classof(targetConstructor)) return; 
    //we don't care copy or move constructor
    if (targetConstructor -> isCopyConstructor() || targetConstructor -> isMoveConstructor())
        return;
    //ok, find explicit keyword
    if (targetConstructor -> isExplicit()) 
        return;
    ASTUtility::Print(targetConstructor, Context, "Rule066");
}
