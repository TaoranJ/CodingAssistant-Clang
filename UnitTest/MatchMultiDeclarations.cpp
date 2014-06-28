#include "ASTUtility.h" 

//match decl stmt
StatementMatcher declStmtMatcher = declStmt().bind("declStmt");

class MultiDeclPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::DeclStmt *declStmt = Result.Nodes.getNodeAs<clang::DeclStmt>("declStmt");

        if(!declStmt || ASTUtility::IsStmtInSTDFile(declStmt, Context)) return;
        
        if( !(declStmt -> isSingleDecl()) ) 
            ASTUtility::Print(declStmt, Context, "Rule010");
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
    MultiDeclPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(declStmtMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
