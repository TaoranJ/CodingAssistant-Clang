#include "ASTUtility.h"

StatementMatcher ExplicitCastMatcher = explicitCastExpr().bind("explicitcastexpr");

class ExplicitCastChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result){
		//get the ASTContext
		clang::ASTContext *Context = Result.Context;
		const clang::ExplicitCastExpr *ece = Result.Nodes.getNodeAs<clang::ExplicitCastExpr>("explicitcastexpr");

		if(!ece || ASTUtility::IsStmtInSTDFile(ece, Context)) return;

        if (clang::CXXReinterpretCastExpr::classof(ece))
            ASTUtility::Print(ece, Context, "Rule045");
        else if (clang::CXXStaticCastExpr::classof(ece))
            ASTUtility::Print(ece, Context, "Rule046");
        else if (clang::CXXConstCastExpr::classof(ece))
            ASTUtility::Print(ece, Context, "Rule047");
        else if (clang::CStyleCastExpr::classof(ece))
            ASTUtility::Print(ece, Context, "Rule048");
	}
};


static llvm::cl::OptionCategory MyToolCategory("options");

//Main @TestNeed
int main(int argc, const char **argv){
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	ExplicitCastChecker ecc;
	MatchFinder finder;
	finder.addMatcher(ExplicitCastMatcher,&ecc);
	Tool.run(newFrontendActionFactory(&finder));
	return 0;
}
