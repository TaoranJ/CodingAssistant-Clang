#include "ASTUtility.h"

StatementMatcher classConstructMatcher = constructExpr(
        hasDescendant(newExpr())).bind("handle");


class NewExprDeclPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::CXXConstructExpr *construct = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("handle");

        if(!construct || ASTUtility::IsStmtInSTDFile(construct, Context)) return;
        
        //if std
        if (ASTUtility::IsDeclInSTDFile(construct -> getConstructor(), Context)) 
            ASTUtility::Print(construct, Context, "Rule019");
        else
            ASTUtility::Print(construct, Context, "Rule020");
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
    NewExprDeclPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(classConstructMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
