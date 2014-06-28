#include "ASTUtility.h" 

//match for-range statement 
StatementMatcher forRangeStmtMatcher = forRangeStmt().bind("forRangeStmt");
//match for statement
StatementMatcher forStmtMatcher = forStmt().bind("forStmt");

class ForRangeStmtPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        clang::ASTContext *Context = Result.Context;
        const clang::CXXForRangeStmt *forRange = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("forRangeStmt");
        const clang::ForStmt *forStmt = Result.Nodes.getNodeAs<clang::ForStmt>("forStmt");

        if(forRange && !ASTUtility::IsStmtInSTDFile(forRange, Context))
            ASTUtility::Print(forRange, Context, "Rule026.ForRange");
        else if (forStmt && !ASTUtility::IsStmtInSTDFile(forStmt, Context))
            ASTUtility::Print(forStmt, Context, "Rule026.For");
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
    ForRangeStmtPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(forRangeStmtMatcher, &Printer);
    Finder.addMatcher(forStmtMatcher, &Printer);
    Tool.run(newFrontendActionFactory(&Finder));

    return 0;
}
