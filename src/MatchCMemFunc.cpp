#include "../include/MatchCMemFunc.h"

namespace CodStd101 {
        //StatementMatcher callExprMatcher = callExpr().bind("callexpr");
        StatementMatcher mallocMatcherCS = callExpr(callee(functionDecl(hasName("malloc")).bind("m"))).bind("mallocCall");
        StatementMatcher freeMatcherCS = callExpr(callee(functionDecl(hasName("free")).bind("f"))).bind("freeCall");
        StatementMatcher reallocMatcherCS = callExpr(callee(functionDecl(hasName("realloc")).bind("r"))).bind("reallocCall");
        
        //memcpy arrayType non-builtin = pointer or user-defined
        StatementMatcher memcpyAryMatcherCS = callExpr(
                callee(functionDecl(hasName("memcpy")).bind("cpy")),
                hasAnyArgument(ignoringImpCasts(declRefExpr(
                            to(declaratorDecl(hasType(arrayType(
                                            unless(hasElementType(builtinType()))).bind("cpyParm")))))))
                ).bind("memcpyCall");
        StatementMatcher memcpyPtrMatcherCS = callExpr(
                callee(functionDecl(hasName("memcpy")).bind("cpy")),
                hasAnyArgument(ignoringImpCasts(declRefExpr(
                            to(declaratorDecl(hasType(pointerType(
                                            pointee(unless(builtinType()))).bind("cpyParmPtr")))))))
                ).bind("memcpyCall");
        
        
        StatementMatcher memcmpAryMatcherCS = callExpr(
                callee(functionDecl(hasName("memcmp")).bind("cmp")),
                hasAnyArgument(ignoringImpCasts(declRefExpr(
                            to(declaratorDecl(hasType(arrayType(
                                            unless(hasElementType(builtinType()))).bind("cmpParm")))))))
                ).bind("memcmpCall");
        StatementMatcher memcmpPtrMatcherCS = callExpr(
                callee(functionDecl(hasName("memcmp")).bind("cmp")),
                hasAnyArgument(ignoringImpCasts(declRefExpr(
                            to(declaratorDecl(hasType(pointerType(
                                            pointee(unless(builtinType()))).bind("cmpParmPtr")))))))
                ).bind("memcmpCall");
}

void CodStd101::CallExprPrinter::run(const MatchFinder::MatchResult &Result)
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
        ASTUtility::Print(mallocCall, Context, "Rule057");
        return;
    }
    if (freeCall && f && ASTUtility::IsDeclInSTDFile(f, Context))
    {
        ASTUtility::Print(freeCall, Context, "Rule057");
        return;
    }
    if(reallocCall && r && ASTUtility::IsDeclInSTDFile(r, Context))
    {
        ASTUtility::Print(reallocCall, Context, "Rule057");
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
            ASTUtility::Print(memcpyCall, Context, "Rule058");
        }
        else if (cpyParmPtr) //ptr
        {
            if (cpyParmPtr -> getPointeeType().isPODType(*Context)) return; 
            ASTUtility::Print(memcpyCall, Context, "Rule058");
        }
        return;
    }

    if(memcmpCall && cmp && ASTUtility::IsDeclInSTDFile(cmp, Context))
    {
        //array
        if (cmpParm)
        {
            if (cmpParm ->  getElementType().isPODType(*Context)) return;
            ASTUtility::Print(memcmpCall, Context, "Rule058");
        }
        else if (cmpParmPtr) //ptr
        {
            if (cmpParmPtr -> getPointeeType().isPODType(*Context)) return; 
            ASTUtility::Print(memcmpCall, Context, "Rule058");
        }
        return;
    }
}
