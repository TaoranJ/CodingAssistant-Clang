#include "../include/MatchOnlyDefaultConstructor.h"

namespace ProgrammingLanguage {

DeclarationMatcher classOnlyDefaultConstructorMatcherPL = recordDecl(
            isDefinition(),
            unless(has(constructorDecl(hasAnyParameter(anything()))))).bind("targetClass");
}

void ProgrammingLanguage::OnlyDefaultConstructorClassPrinter::run(const MatchFinder::MatchResult &Result)
{
    clang::ASTContext *Context = Result.Context;
    const clang::CXXRecordDecl *targetClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("targetClass");
    
    if (!targetClass || ASTUtility::IsDeclInSTDFile(targetClass, Context))
        return;
    
    ASTUtility::Print(targetClass, Context, "Rule065");
}
