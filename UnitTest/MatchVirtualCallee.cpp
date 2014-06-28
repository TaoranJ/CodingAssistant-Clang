#include "ASTUtility.h"

StatementMatcher ctorCallVtlMatcher = compoundStmt(
        hasParent(constructorDecl().bind("cDecl")),
        hasDescendant(callExpr(callee(methodDecl(isVirtual()))))
        );

StatementMatcher dtorCallVtlMatcher = compoundStmt(
        hasParent(destructorDecl().bind("dDecl")),
        hasDescendant(callExpr(callee(methodDecl(isVirtual()))))
        );

class CtorCallVtlChecker : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result)
    {
		clang::ASTContext *Context = Result.Context;
        const clang::CXXConstructorDecl *construct = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("cDecl");
        const clang::CXXDestructorDecl *destruct = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("dDecl");

        if (construct && !ASTUtility::IsDeclInSTDFile(construct, Context))
            ASTUtility::Print(construct, Context, "Rule055");
        if (destruct && !ASTUtility::IsDeclInSTDFile(destruct, Context))
            ASTUtility::Print(destruct, Context, "Rule055");
    }
};


static llvm::cl::OptionCategory MyToolCategory("options");

//Main @TestNeed
int main(int argc, const char **argv){
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	MatchFinder finder;
    CtorCallVtlChecker Printer;   
    finder.addMatcher(ctorCallVtlMatcher, &Printer);
    finder.addMatcher(dtorCallVtlMatcher, &Printer);

	Tool.run(newFrontendActionFactory(&finder));
}
