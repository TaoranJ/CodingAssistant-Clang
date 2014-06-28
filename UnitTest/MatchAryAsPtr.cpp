#include "ASTUtility.h"

StatementMatcher callExprMatcher = callExpr(
        findAll(implicitCastExpr().bind("imp"))).bind("expr");

class ArrayPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::ImplicitCastExpr *impCast = Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("imp");
        const clang::CallExpr *call = Result.Nodes.getNodeAs<clang::CallExpr>("expr");

        if(!impCast || ASTUtility::IsStmtInSTDFile(call, Context)) return;

        std::string str = "ArrayToPointerDecay";
        if( str == impCast -> getCastKindName() )
            ASTUtility::Print(call, Context, "Rule040");
    }
};

//"My tool options" is the name.
static llvm::cl::OptionCategory MyToolCategory("My tool options");

int main(int argc, const char **argv)
{
    //CommonOptionsParser constructor will parse arguments and create a
    //CompilationDatabase. In case of error it will terminate the program.
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    //once we have a CompilationDatabase, we can create a ClangTool and run our
    //FrontendAction over some code.
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    //tooling::MyFactory Factory;
    ArrayPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(callExprMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
