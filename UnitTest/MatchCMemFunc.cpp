#include "ASTUtility.h"

//StatementMatcher callExprMatcher = callExpr().bind("callexpr");
StatementMatcher mallocMatcher = callExpr(callee(functionDecl(hasName("malloc")).bind("m"))).bind("mallocCall");
StatementMatcher freeMatcher = callExpr(callee(functionDecl(hasName("free")).bind("f"))).bind("freeCall");
StatementMatcher reallocMatcher = callExpr(callee(functionDecl(hasName("realloc")).bind("r"))).bind("reallocCall");

//memcpy arrayType non-builtin = pointer or user-defined
StatementMatcher memcpyAryMatcher = callExpr(
        callee(functionDecl(hasName("memcpy")).bind("cpy")),
        hasAnyArgument(ignoringImpCasts(declRefExpr(
                    to(declaratorDecl(hasType(arrayType(
                                    unless(hasElementType(builtinType()))).bind("cpyParm")))))))
        ).bind("memcpyCall");
StatementMatcher memcpyPtrMatcher = callExpr(
        callee(functionDecl(hasName("memcpy")).bind("cpy")),
        hasAnyArgument(ignoringImpCasts(declRefExpr(
                    to(declaratorDecl(hasType(pointerType(
                                    pointee(unless(builtinType()))).bind("cpyParmPtr")))))))
        ).bind("memcpyCall");


StatementMatcher memcmpAryMatcher = callExpr(
        callee(functionDecl(hasName("memcmp")).bind("cmp")),
        hasAnyArgument(ignoringImpCasts(declRefExpr(
                    to(declaratorDecl(hasType(arrayType(
                                    unless(hasElementType(builtinType()))).bind("cmpParm")))))))
        ).bind("memcmpCall");
StatementMatcher memcmpPtrMatcher = callExpr(
        callee(functionDecl(hasName("memcmp")).bind("cmp")),
        hasAnyArgument(ignoringImpCasts(declRefExpr(
                    to(declaratorDecl(hasType(pointerType(
                                    pointee(unless(builtinType()))).bind("cmpParmPtr")))))))
        ).bind("memcmpCall");


class CallExprChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		clang::ASTContext *Context = Result.Context;
        //Rule 61
		const clang::CallExpr *mallocCall = Result.Nodes.getNodeAs<clang::CallExpr>("mallocCall");
		const clang::CallExpr *freeCall = Result.Nodes.getNodeAs<clang::CallExpr>("freeCall");
		const clang::CallExpr *reallocCall = Result.Nodes.getNodeAs<clang::CallExpr>("reallocCall");
		const clang::FunctionDecl *m = Result.Nodes.getNodeAs<clang::FunctionDecl>("m");
		const clang::FunctionDecl *f = Result.Nodes.getNodeAs<clang::FunctionDecl>("f");
		const clang::FunctionDecl *r = Result.Nodes.getNodeAs<clang::FunctionDecl>("r");

        if (mallocCall && m && ASTUtility::IsDeclInSTDFile(m, Context))
        {
            ASTUtility::Print(mallocCall, Context, "Rule061");
            return;
        }
        if (freeCall && f && ASTUtility::IsDeclInSTDFile(f, Context))
        {
            ASTUtility::Print(freeCall, Context, "Rule061");
            return;
        }
        if(reallocCall && r && ASTUtility::IsDeclInSTDFile(r, Context))
        {
            ASTUtility::Print(reallocCall, Context, "Rule061");
            return;
        }

        //Rule 62
		const clang::CallExpr *memcpyCall = Result.Nodes.getNodeAs<clang::CallExpr>("memcpyCall");
        const clang::FunctionDecl* cpy = Result.Nodes.getNodeAs<clang::FunctionDecl>("cpy");
        const clang::ArrayType* cpyParm = Result.Nodes.getNodeAs<clang::ArrayType>("cpyParm");
        const clang::PointerType* cpyParmPtr = Result.Nodes.getNodeAs<clang::PointerType>("cpyParmPtr");

		const clang::CallExpr *memcmpCall = Result.Nodes.getNodeAs<clang::CallExpr>("memcmpCall");
        const clang::FunctionDecl* cmp = Result.Nodes.getNodeAs<clang::FunctionDecl>("cmp");
        const clang::ArrayType* cmpParm = Result.Nodes.getNodeAs<clang::ArrayType>("cmpParm");
        const clang::PointerType* cmpParmPtr = Result.Nodes.getNodeAs<clang::PointerType>("cmpParmPtr");
        

        if (memcpyCall && cpy && ASTUtility::IsDeclInSTDFile(cpy, Context))
        {
            //array
            if (cpyParm)
            {
                if (cpyParm ->  getElementType().isPODType(*Context)) return;
                ASTUtility::Print(memcpyCall, Context, "Rule062");
            }
            else if (cpyParmPtr) //ptr
            {
                if (cpyParmPtr -> getPointeeType().isPODType(*Context)) return; 
                ASTUtility::Print(memcpyCall, Context, "Rule062");
            }
            return;
        }

        if(memcmpCall && cmp && ASTUtility::IsDeclInSTDFile(cmp, Context))
        {
            //array
            if (cmpParm)
            {
                if (cmpParm ->  getElementType().isPODType(*Context)) return;
                ASTUtility::Print(memcmpCall, Context, "Rule062");
            }
            else if (cmpParmPtr) //ptr
            {
                if (cmpParmPtr -> getPointeeType().isPODType(*Context)) return; 
                ASTUtility::Print(memcmpCall, Context, "Rule062");
            }
            return;
        }
	}
};

static llvm::cl::OptionCategory MyToolCategory("options");

//Main @TestNeed
int main(int argc, const char **argv){
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	CallExprChecker cec;
	MatchFinder finder;
	finder.addMatcher(mallocMatcher, &cec);
	finder.addMatcher(freeMatcher, &cec);
	finder.addMatcher(reallocMatcher, &cec);
	finder.addMatcher(memcpyAryMatcher, &cec);
	finder.addMatcher(memcpyPtrMatcher, &cec);
	finder.addMatcher(memcmpAryMatcher, &cec);
	finder.addMatcher(memcmpPtrMatcher, &cec);
	Tool.run(newFrontendActionFactory(&finder));
	return 0;
} 
