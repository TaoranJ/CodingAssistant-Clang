#include "../include/MatchUninline.h"

namespace ProgrammingLanguage {
    DeclarationMatcher functionDeclMatcherPL = functionDecl(isDefinition()).bind("functiondecl");
}

void ProgrammingLanguage::UnInlineFuncPrinter::run(const MatchFinder::MatchResult &Result)
{
	//get the ASTContext
	clang::ASTContext *Context = Result.Context;
	const clang::FunctionDecl *fd = Result.Nodes.getNodeAs<clang::FunctionDecl>("functiondecl");

	if (!fd || ASTUtility::IsDeclInSTDFile(fd, Context)) return;
    //not .h
    if (fd -> getLocStart().printToString(Context -> getSourceManager()).find(".h") == std::string::npos) return;
    
    //non #include
    if(!fd -> isInlineSpecified())
        ASTUtility::Print(fd, Context, "Rule054");
}
