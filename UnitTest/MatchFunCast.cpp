#include "ASTUtility.h"

StatementMatcher funcCastMatcher = functionalCastExpr().bind("funcCast");

class functionalCastPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::CXXFunctionalCastExpr *funCast = Result.Nodes.getNodeAs<clang::CXXFunctionalCastExpr>("funcCast");

        if(!funCast || ASTUtility::IsStmtInSTDFile(funCast, Context)) return;
        
        ASTUtility::Print(funCast, Context, "Rule034");
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
    functionalCastPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(funcCastMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
