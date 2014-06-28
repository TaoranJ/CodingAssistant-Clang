#include "ASTUtility.h" 

// int *x = NULL or int *x = 0
DeclarationMatcher nullPointerMatcher = varDecl(hasType(pointerType()),
        hasInitializer(implicitCastExpr().bind("cast"))).bind("var");

// x == NULL  or x == 0
StatementMatcher biOpMatcher1 = binaryOperator(hasRHS(implicitCastExpr().bind("castR1")),
        hasOperatorName("==")).bind("bo1");

// x != NULL or x != 0
StatementMatcher biOpMatcher2 = binaryOperator(hasRHS(implicitCastExpr().bind("castR2")),
        hasOperatorName("!=")).bind("bo2");

// x != NULL or x != 0
StatementMatcher biOpMatcher3 = binaryOperator(hasRHS(implicitCastExpr().bind("castR3")),
        hasOperatorName("=")).bind("bo3");

class NullPointerPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
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
};

//"My tool options" is the name.
static llvm::cl::OptionCategory MyToolCategory("My tool options");

int main(int argc, const char **argv)
{
    //
    //CommonOptionsParser constructor will parse arguments and create a
    //CompilationDatabase. In case of error it will terminate the program.
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    //once we have a CompilationDatabase, we can create a ClangTool and run our
    //FrontendAction over some code.
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    //tooling::MyFactory Factory;
    NullPointerPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(nullPointerMatcher, &Printer);
    Finder.addMatcher(biOpMatcher1, &Printer);
    Finder.addMatcher(biOpMatcher2, &Printer);
    Finder.addMatcher(biOpMatcher3, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
