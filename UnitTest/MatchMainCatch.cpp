#include "ASTUtility.h"

//find func with try catch in it
StatementMatcher tryInFuncMatcher = compoundStmt(
        hasParent(functionDecl().bind("func")),         
        hasDescendant(tryStmt())).bind("funcBody");


class TryInFuncPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::CompoundStmt *funcBody = Result.Nodes.getNodeAs<clang::CompoundStmt>("funcBody");
        const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("func");

        if(!funcBody || !func || ASTUtility::IsStmtInSTDFile(funcBody, Context)) return;

        if(func -> isMain()) ASTUtility::Print(funcBody, Context, "Rule006");        
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
    TryInFuncPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(tryInFuncMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
