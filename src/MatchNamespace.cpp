#include "../include/MatchNamespace.h"

namespace ProgrammingLanguage {

    //match namespace 
    DeclarationMatcher namespaceMatcherPL = namespaceDecl().bind("namespaceDecl");
}

void ProgrammingLanguage::NamespacePrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::NamespaceDecl *scope = Result.Nodes.getNodeAs<clang::NamespaceDecl>("namespaceDecl");

    if(!scope || ASTUtility::IsDeclInSTDFile(scope, Context)) return;
    
    std::string name = scope -> getNameAsString();
    
    //short: <= 3 but not the anonymous namespace
    if (name.length() <= 3 && name.length() != 0)
        ASTUtility::Print(scope, Context, "Rule043");
}
