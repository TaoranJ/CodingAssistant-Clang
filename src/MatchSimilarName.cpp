#include "../include/MatchSimilarName.h"

namespace ProgrammingLanguage {
    const int lengthThreshold = 2;
    int similarPairCount = 0;
    std::vector<std::string> nameVec;
    DeclarationMatcher varNameMatcherPL = namedDecl().bind("vardecl");
}

void ProgrammingLanguage::SimilarNamePrinter::run(const MatchFinder::MatchResult &Result)
{
	//get the ASTContext
	clang::ASTContext *Context = Result.Context;
	//get the VarASTNode
	const clang::NamedDecl *decl = Result.Nodes.getNodeAs<clang::NamedDecl>("vardecl");
	//TODO handle the condition "without vaild data"
	if(!decl || ASTUtility::IsDeclInSTDFile(decl, Context) || clang::ParmVarDecl::classof(decl)) return;

    //decl may be fieldecl or record decl or var decl
	std::string varName(decl -> getNameAsString());
	if(varName.size() > lengthThreshold)
		nameVec.push_back(varName);		
}
