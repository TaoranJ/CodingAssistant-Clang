#include "ExceptNumber.h"
#include "ASTUtility.h"
 

//TODO @TestNeed
static int count = 0;

//Declare a matcher of ASTNode to match magic number set
StatementMatcher FloatLiteralMatcher = floatLiteral().bind("floatliteral");

StatementMatcher IntLiteralMatcher = integerLiteral().bind("intliteral");

StatementMatcher StringLiteralMatcher = stringLiteral().bind("stringliteral");



//Declare a class Inherit clang::ast_matchers::MatchFinder::MatchCallback
class FloatChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		//get the ASTContext
		clang::ASTContext *Context = Result.Context;
		//get the MagicNumberASTNode
		const clang::FloatingLiteral *fl = Result.Nodes.getNodeAs<clang::FloatingLiteral>("floatliteral");	
		//TODO handle the condition "without vaild data"
		if(!fl || ASTUtility::IsStmtInSTDFile(fl, Context)) return;
        ASTUtility::Print(fl, Context, "Rule049");
	}
};

class IntChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		//get the ASTContext
		clang::ASTContext *Context = Result.Context;
		//get the IntegerLiteral ASTNode
		const clang::IntegerLiteral *il = Result.Nodes.getNodeAs<clang::IntegerLiteral>("intliteral");
		//TODO handle the condition "without "	
		if(!il || ASTUtility::IsStmtInSTDFile(il, Context)) return;

		//TODO get the IntegerLiteral value
		llvm::APInt value = il->getValue();
		//TODO remove the except int from ExceptNumber.h
		for(int i = 0; i < IntExcept.size(); ++i)
        {
            if(IntExcept[i] == value) return; 
        }
        ASTUtility::Print(il, Context, "Rule049");
	}
};

class StringChecker : public MatchFinder::MatchCallback {
public:	
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		//get the ASTContext
		clang::ASTContext *Context = Result.Context;
		//get the IntegerLiteral ASTNode
		const clang::StringLiteral *sl = Result.Nodes.getNodeAs<clang::StringLiteral>("stringliteral");
		if(!sl || ASTUtility::IsStmtInSTDFile(sl, Context)) return;	

		llvm::StringRef value = sl->getString();
		//TODO remove the except string from ExceptNumber.h
		for(int i = 0; i < StringExcept.size(); ++i)
        {
			if(StringExcept[i].compare(value) == 0) return;
		}	
        ASTUtility::Print(sl, Context, "Rule049");
	}
};

//TODO will implement in the future when to integrate the Code
//@TestNeed
static llvm::cl::OptionCategory MyToolCategory("options");

/*
 *Main @TestNeed
 */

int main(int argc, const char **argv){
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);

	//Create an ClangTool instance to run a FrontendAction over a set of files
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	
	//tooling::MyFactory Factory;
	FloatChecker fc;
	IntChecker ic;
	StringChecker sc;
	MatchFinder finder;
	
	finder.addMatcher(FloatLiteralMatcher , &fc);
	finder.addMatcher(IntLiteralMatcher , &ic);
	finder.addMatcher(StringLiteralMatcher , &sc);
	Tool.run(newFrontendActionFactory(&finder));

	llvm::outs() << "Avoid magic number : " << count << "\n";
	return 0;
}

