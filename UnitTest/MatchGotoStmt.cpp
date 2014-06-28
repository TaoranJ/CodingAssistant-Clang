#include "ASTUtility.h" 

//match do statement
StatementMatcher gotoStmtMatcher = gotoStmt().bind("gotoStmt");

class GotoStmtPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::GotoStmt *gtStmt = Result.Nodes.getNodeAs<clang::GotoStmt>("gotoStmt");

        if(!gtStmt || ASTUtility::IsStmtInSTDFile(gtStmt, Context)) return;

        ASTUtility::Print(gtStmt, Context, "Rule028");
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
    GotoStmtPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(gotoStmtMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
