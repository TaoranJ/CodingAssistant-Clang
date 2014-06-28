#include "../include/MatchMagicNumber.h"

namespace CodStd101 {

    StatementMatcher floatLiteralMatcherCS = floatLiteral().bind("floatliteral");
    StatementMatcher intLiteralMatcherCS = integerLiteral().bind("intliteral");
    StatementMatcher stringLiteralMatcherCS = stringLiteral().bind("stringliteral");
}

void CodStd101::FloatPrinter::run(const MatchFinder::MatchResult &Result)
{
	//get the ASTContext
	clang::ASTContext *Context = Result.Context;
	//get the MagicNumberASTNode
	const clang::FloatingLiteral *fl = Result.Nodes.getNodeAs<clang::FloatingLiteral>("floatliteral");	

	if(!fl || ASTUtility::IsStmtInSTDFile(fl, Context)) return;
    ASTUtility::Print(fl, Context, "Rule049");
}

void CodStd101::IntPrinter::run(const MatchFinder::MatchResult &Result)
{
	//get the ASTContext
	clang::ASTContext *Context = Result.Context;
	//get the IntegerLiteral ASTNode
	const clang::IntegerLiteral *il = Result.Nodes.getNodeAs<clang::IntegerLiteral>("intliteral");
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

void CodStd101::StringPrinter::run(const MatchFinder::MatchResult &Result)
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
