#include "ASTUtility.h"

DeclarationMatcher varDeclMatcher = varDecl(
        hasInitializer(newExpr())).bind("var");

class NewExprDeclPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::VarDecl *var = Result.Nodes.getNodeAs<clang::VarDecl>("var");

        if(!var || ASTUtility::IsDeclInSTDFile(var, Context)) return; 

        //if(var -> hasLocalStorage())
        ASTUtility::Print(var, Context, "Rule021");
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
    NewExprDeclPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(varDeclMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
