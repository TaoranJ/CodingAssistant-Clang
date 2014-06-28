#include "../include/MatchUsing.h"

namespace ProgrammingLanguage {
    //only using std:vector
    //no using namespace std;
    DeclarationMatcher usingDeclMatcherPL = usingDecl().bind("usingDecl");
}

bool ProgrammingLanguage::CheckHeader(std::string fileName) 
{
	if(fileName.find(".hh") == fileName.size() - 3 ||
		fileName.find(".h") == fileName.size() - 2 ||
        fileName.find(".H") == fileName.size() - 2 ||
        fileName.find(".hpp") == fileName.size() - 4)
		return true;
	return false;
}

void ProgrammingLanguage::UsingPrinter::run(const MatchFinder::MatchResult &Result)
{
	clang::ASTContext *Context = Result.Context;
	const clang::UsingDecl *ud = Result.Nodes.getNodeAs<clang::UsingDecl>("usingDecl");

	if(!ud || ASTUtility::IsDeclInSTDFile(ud, Context)) return;
    std::string loc = ud -> getUsingLoc().printToString(Context -> getSourceManager());

    if (!ud -> getQualifier() -> getAsNamespace() -> getOriginalNamespace() -> getNameAsString().compare("std"))
        return;
    if (!CheckHeader(loc.substr(0, loc.find(":")))) return;

	ASTUtility::Print(ud, Context, "Rule064");
}
