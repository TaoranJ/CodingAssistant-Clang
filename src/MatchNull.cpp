#include "../include/MatchNull.h" 

namespace ProgrammingLanguage {

    // int *x = NULL or int *x = 0
    DeclarationMatcher nullPointerMatcherPL = varDecl(hasType(pointerType()),
            hasInitializer(implicitCastExpr().bind("cast"))).bind("var");
    // x == NULL  or x == 0
    StatementMatcher biOpMatcher1PL = binaryOperator(hasRHS(implicitCastExpr().bind("castR1")),
            hasOperatorName("==")).bind("bo1");
    // x != NULL or x != 0
    StatementMatcher biOpMatcher2PL = binaryOperator(hasRHS(implicitCastExpr().bind("castR2")),
            hasOperatorName("!=")).bind("bo2");
    // x != NULL or x != 0
    StatementMatcher biOpMatcher3PL = binaryOperator(hasRHS(implicitCastExpr().bind("castR3")),
            hasOperatorName("=")).bind("bo3");
}

void ProgrammingLanguage::NullPointerPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::ImplicitCastExpr *cast = Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("cast");
    const clang::ImplicitCastExpr *castR1 = Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("castR1");
    const clang::ImplicitCastExpr *castR2 = Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("castR2");
    const clang::ImplicitCastExpr *castR3 = Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("castR3");
    const clang::BinaryOperator *bo1 = Result.Nodes.getNodeAs<clang::BinaryOperator>("bo1");
    const clang::BinaryOperator *bo2 = Result.Nodes.getNodeAs<clang::BinaryOperator>("bo2");
    const clang::BinaryOperator *bo3 = Result.Nodes.getNodeAs<clang::BinaryOperator>("bo3");
    const clang::VarDecl *var = Result.Nodes.getNodeAs<clang::VarDecl>("var");

    //situation 1: 
    if(var && cast && !ASTUtility::IsDeclInSTDFile(var, Context))
    {
        for(const auto i : const_cast<clang::ImplicitCastExpr*>(cast) -> children())
        {
            if (clang::GNUNullExpr::classof(i) || clang::IntegerLiteral::classof(i))
                ASTUtility::Print(var, Context, "Rule017");
        }
    }

    if (bo1 && castR1 && !ASTUtility::IsStmtInSTDFile(bo1, Context))
    {
        for(const auto i : const_cast<clang::ImplicitCastExpr*>(castR1) -> children())
        {
            if (clang::GNUNullExpr::classof(i) || clang::IntegerLiteral::classof(i))
                ASTUtility::Print(bo1, Context, "Rule017");
        }
    }

    if (bo2 && castR2 && !ASTUtility::IsStmtInSTDFile(bo2, Context))
    {
        for(const auto i : const_cast<clang::ImplicitCastExpr*>(castR2) -> children())
        {
            if (clang::GNUNullExpr::classof(i) || clang::IntegerLiteral::classof(i))
                ASTUtility::Print(bo2, Context, "Rule017");
        }
    }

    if (bo3 && castR3 && !ASTUtility::IsStmtInSTDFile(bo3, Context))
    {
        for(const auto i : const_cast<clang::ImplicitCastExpr*>(castR3) -> children())
        {
            if (clang::GNUNullExpr::classof(i) || clang::IntegerLiteral::classof(i))
                ASTUtility::Print(bo3, Context, "Rule017");
        }
    }
}
