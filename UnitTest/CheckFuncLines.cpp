/*
 *检查过长的方法
 */
#include "ASTUtility.h"
#include <iostream>

//threshold value , it can be changed in configure file
const int funcThreshold = 35;

StatementMatcher FuncStmtMatcher = compoundStmt(
				hasParent(functionDecl().bind("functiondecl"))).bind("funcstmt");

class FuncLinesChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result){
		//get the ASTContext
		clang::ASTContext *Context = Result.Context;
		//get the CompoundStmt Node Relate to Function
		const clang::CompoundStmt *cs = Result.Nodes.getNodeAs<clang::CompoundStmt>("funcstmt");
		const clang::FunctionDecl *fd = Result.Nodes.getNodeAs<clang::FunctionDecl>("functiondecl");

		if(!cs || !fd || !Context->getSourceManager().isInMainFile(cs->getLBracLoc()) || ASTUtility::IsDeclInSTDFile(fd, Context))
			return;

		clang::FullSourceLoc startLoc = Context->getFullLoc(cs->getLocStart());
		clang::FullSourceLoc endLoc = Context->getFullLoc(cs->getLocEnd());
		if(startLoc.isValid() && endLoc.isValid())
        {
			int startLine = startLoc.getSpellingLineNumber();
			int endLine = endLoc.getSpellingLineNumber();
            if (endLine - startLine >= funcThreshold)
            {
                ASTUtility::Print(fd, Context, "Rule050");
            }
		}
	}
};

static llvm::cl::OptionCategory MyToolCategory("options");

//Main @TestNeed
int main(int argc, const char **argv){
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	FuncLinesChecker flc;
	MatchFinder finder;
	finder.addMatcher(FuncStmtMatcher,&flc);
	Tool.run(newFrontendActionFactory(&finder));
	
	return 0;
}
