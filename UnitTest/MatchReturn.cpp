#include "ASTUtility.h"

StatementMatcher FuncStmtMatcher = compoundStmt(
				hasParent(functionDecl(returns(pointerType())).bind("functiondecl")),
                hasDescendant(returnStmt(
                        hasDescendant(declRefExpr().bind("decl"))))
                ).bind("funcstmt");

class ReturnChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		clang::ASTContext *Context = Result.Context;
		const clang::CompoundStmt *cs = Result.Nodes.getNodeAs<clang::CompoundStmt>("funcstmt");
		const clang::FunctionDecl *fd = Result.Nodes.getNodeAs<clang::FunctionDecl>("functiondecl");
        const clang::DeclRefExpr *decl = Result.Nodes.getNodeAs<clang::DeclRefExpr>("decl");

		if(!cs || !fd || !decl || ASTUtility::IsStmtInSTDFile(cs, Context)) return;

        if (!clang::VarDecl::classof(decl -> getDecl())) return; 
        
        const clang::VarDecl *var = static_cast<const clang::VarDecl*>(decl -> getDecl());


        if (var -> hasLocalStorage()) 
            ASTUtility::Print(decl, Context, "Rule65");
	}
};

//@TestNeed
static llvm::cl::OptionCategory MyToolCategory("options");

/*
 * Main @TestNeed
 */
int main(int argc, const char **argv){
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	ReturnChecker rc;
	MatchFinder finder;
	finder.addMatcher(FuncStmtMatcher, &rc);
	Tool.run(newFrontendActionFactory(&finder));	
	return 0;
}

