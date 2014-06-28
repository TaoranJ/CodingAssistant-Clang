#include "ASTUtility.h"
#include "StringProcess.h"
#include <fstream>

using namespace clang::driver;
using namespace clang::tooling;
using namespace clang::ast_matchers;

const int lengthThreshold = 2;
int similarPairCount = 0;
std::vector<std::string> nameVec;
//Declare a matcher of ASTNode to match var;
DeclarationMatcher VarNameMatcher = namedDecl().bind("vardecl");

class SimilarNameChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
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
};

//@TestNeed
static llvm::cl::OptionCategory MyToolCategory("options");

/*
 * Main @TestNeed
 */
int main(int argc, const char **argv){
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	//Create an ClangTool instance to run a FrontendAction over a set of files
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	//tool::MyFactory Factory;
	SimilarNameChecker snc;
	MatchFinder finder;
	finder.addMatcher(VarNameMatcher, &snc);
	Tool.run(newFrontendActionFactory(&finder));

	for(int i = 0; i < nameVec.size(); ++i)
    {
		std::string oneName(nameVec[i]);
		for(int j = i + 1; j < nameVec.size(); ++j)
        {
			std::string anotherName(nameVec[j]);
			if(calculateSimilarity(oneName,anotherName) >= 0.5 && calculateSimilarity(oneName,anotherName) != 1)
            {
                ofstream out("Rule053", std::ostream::app);
                out << oneName << " : " << anotherName << "\n";
			}
		}
	}
	return 0;
}
