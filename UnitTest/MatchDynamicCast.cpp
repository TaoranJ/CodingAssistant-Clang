#include "ASTUtility.h"

StatementMatcher dynamicCastMatcher = dynamicCastExpr().bind("dynCast");

class DynamicCastPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::CXXDynamicCastExpr *dynCast = Result.Nodes.getNodeAs<clang::CXXDynamicCastExpr>("dynCast");

        if(!dynCast && ASTUtility::IsStmtInSTDFile(dynCast, Context)) return;
        
        ASTUtility::Print(dynCast, Context, "Rule035");
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
    DynamicCastPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(dynamicCastMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
