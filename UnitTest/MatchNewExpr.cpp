#include "ASTUtility.h"

StatementMatcher cxxNewExprMatcher = newExpr().bind("newExpr");

class NewExprPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::CXXNewExpr *newExpr = Result.Nodes.getNodeAs<clang::CXXNewExpr>("newExpr");
        
        if(!newExpr || ASTUtility::IsStmtInSTDFile(newExpr, Context)) return;

        ASTUtility::Print(newExpr, Context, "Rule031");
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
    NewExprPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(cxxNewExprMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
