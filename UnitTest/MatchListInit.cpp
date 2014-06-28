#include "ASTUtility.h" 

//int x = {2}     or int x{2}
DeclarationMatcher varDeclMatcher = varDecl(hasInitializer(initListExpr())).bind("var"); 

//for class
DeclarationMatcher classVarDeclMatcher = varDecl(hasInitializer(constructExpr(isListInitialization()))).bind("varClass");

class InitListPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run (const MatchFinder::MatchResult& Result)
    {
        clang::ASTContext* Context = Result.Context;
        const clang::VarDecl* varNormal = Result.Nodes.getNodeAs<clang::VarDecl>("var");
        const clang::VarDecl* varClass = Result.Nodes.getNodeAs<clang::VarDecl>("varClass");

        if (varNormal && !ASTUtility::IsDeclInSTDFile(varNormal, Context))
            ASTUtility::Print(varNormal, Context, "Rule013"); 
        else if (varClass || ASTUtility::IsDeclInSTDFile(varClass, Context))
        {
            ASTUtility::Print(varClass, Context, "Rule014"); 
            std::ofstream out("Rule014.back");
            if(!out) return;
            out << "true" << std::endl;
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
    InitListPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(classVarDeclMatcher, &Printer);
    Finder.addMatcher(varDeclMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
