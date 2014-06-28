#include "ASTUtility.h"

//find func with try catch in it
StatementMatcher callExprMatcher = callExpr(callee(
            functionDecl(hasName("realloc")))).bind("callFun");

class CallExprPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::CallExpr *callFun = Result.Nodes.getNodeAs<clang::CallExpr>("callFun");

        if(!callFun || ASTUtility::IsStmtInSTDFile(callFun, Context)) return;

        ASTUtility::Print(callFun, Context, "Rule007");        
    }
};

//"My tool options" is the name.
static llvm::cl::OptionCategory MyToolCategory("My tool options");

//static llvm::cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
//static llvm::cl::extrahelp MoreHelp("\nMore help text...");

/******************************************************************************
 *
 *****************************************************************************/
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
    CallExprPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(callExprMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
