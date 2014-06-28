#include "ASTUtility.h"

//@TestNeed
const int globalVarLength = 7;
//Declare a matcher of ASTNode to match var;
DeclarationMatcher VarDeclMatcher = varDecl().bind("vardecl");

//Declare a class Inherit clang::ast_matchers::MatchFinder::MatchCallback
class GlobalVarChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result){
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
	GlobalVarChecker gvc;
	MatchFinder finder;
	finder.addMatcher(VarDeclMatcher, &gvc);
	Tool.run(newFrontendActionFactory(&finder));
	return 0;
}
