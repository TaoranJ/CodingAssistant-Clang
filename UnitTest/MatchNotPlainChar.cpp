#include "ASTUtility.h" 

DeclarationMatcher notPlainCharMatcher = declaratorDecl().bind("var");

class NotPlainCharPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        clang::ASTContext *Context = Result.Context;
        const clang::DeclaratorDecl *var = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("var");

        if(!var || ASTUtility::IsDeclInSTDFile(var, Context)) return;
        
        //function
        if( clang::FunctionDecl::classof(var) )
        {
            std::string returnType = static_cast<const clang::FunctionDecl*>(var) -> getReturnType().getAsString();
            if (returnType.find("unsigned char") != std::string::npos || returnType.find("signed char") != std::string::npos) 
                ASTUtility::Print(var, Context, "Rule008");
        }
        else
        {
            std::string type = var -> getTypeSourceInfo() -> getType().getAsString();
            if (type.find("signed char") != std::string::npos || type.find("unsigned char") != std::string::npos) 
                ASTUtility::Print(var, Context, "Rule008");
        }
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
    NotPlainCharPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(notPlainCharMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
