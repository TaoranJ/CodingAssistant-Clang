#include "ASTUtility.h"

const std::string inlineStr("inline");
int unInlineFuncCount;

DeclarationMatcher functionDeclMatcher = functionDecl(isDefinition()).bind("functiondecl");

class UnInlineFuncChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		//get the ASTContext
		clang::ASTContext *Context = Result.Context;
		const clang::FunctionDecl *fd = Result.Nodes.getNodeAs<clang::FunctionDecl>("functiondecl");

		if(!fd || ASTUtility::IsDeclInSTDFile(fd, Context)) return;
        
        //non #include
        if(!fd -> isInlineSpecified())
            ASTUtility::Print(fd, Context, "Rule054");
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
	UnInlineFuncChecker uifc;
	MatchFinder finder;
	finder.addMatcher(functionDeclMatcher, &uifc);
	Tool.run(newFrontendActionFactory(&finder));
	return 0;
}
