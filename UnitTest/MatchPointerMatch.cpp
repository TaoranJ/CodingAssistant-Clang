#include "ASTUtility.h"

StatementMatcher binaryOperatorMatcher = binaryOperator(
        hasLHS(ignoringImpCasts(declRefExpr(hasType(pointerType())))),
        hasRHS(ignoringImpCasts(integerLiteral()))).bind("binaryoperator");

class PtrMemOpChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		clang::ASTContext *Context = Result.Context;
		const clang::BinaryOperator *bop = Result.Nodes.getNodeAs<clang::BinaryOperator>("binaryoperator");
        
		if(!bop || ASTUtility::IsStmtInSTDFile(bop, Context) || bop -> isRelationalOp()) return;

		ASTUtility::Print(bop, Context, "Rule060");
	}
};

static llvm::cl::OptionCategory MyToolCategory("options");

//Main @TestNeed
int main(int argc, const char **argv){
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	PtrMemOpChecker pmoc;
	MatchFinder finder;
	finder.addMatcher(binaryOperatorMatcher,&pmoc);
	Tool.run(newFrontendActionFactory(&finder));
}


