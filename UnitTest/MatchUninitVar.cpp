#include "ASTUtility.h" 

DeclarationMatcher uninitializedVarMatcher = varDecl(
        unless(hasInitializer(expr()))).bind("var");

class UninitVarPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::VarDecl *var = Result.Nodes.getNodeAs<clang::VarDecl>("var");

        if(!var || ASTUtility::IsDeclInSTDFile(var, Context)) return;
        
  //      if( !(var -> hasInit()) ) 
        ASTUtility::Print(var, Context, "Rule011");
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
    UninitVarPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(uninitializedVarMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
