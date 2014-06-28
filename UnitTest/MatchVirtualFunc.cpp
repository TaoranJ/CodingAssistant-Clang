#include "ASTUtility.h"

DeclarationMatcher pubVtlFuncMatcher = methodDecl(isPublic(), isVirtual()).bind("pubVtl");

class PubVtlPrinter : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		clang::ASTContext *Context = Result.Context;
		const clang::CXXMethodDecl *cxxmd = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("pubVtl");

		if(!cxxmd || ASTUtility::IsDeclInSTDFile(cxxmd, Context)) return;

		ASTUtility::Print(cxxmd, Context, "Rule66");
	}
};

//@TestNeed
static llvm::cl::OptionCategory MyToolCategory("options");

/*
 * Main @TestNeed
 */
int main(int argc, const char **argv) {
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	PubVtlPrinter pfc;
	MatchFinder finder;
	finder.addMatcher(pubVtlFuncMatcher, &pfc);
	Tool.run(newFrontendActionFactory(&finder));
	return 0;
}

