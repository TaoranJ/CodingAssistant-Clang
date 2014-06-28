#include "ASTUtility.h" 

DeclarationMatcher autoVarMatcher = varDecl().bind("var");

class AutoVarPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::VarDecl *var = Result.Nodes.getNodeAs<clang::VarDecl>("var");

        if(!var || ASTUtility::IsDeclInSTDFile(var, Context) ) return;

        //not auto, return
        if(!var -> getType() -> getContainedAutoType()) return;

        std::ofstream out("Rule012.back");
        if (!out) return;
        out << "true" << std::endl;

        //auto without =
        std::string expr = ASTUtility::Print(var, Context);

        if(expr.find("=") == std::string::npos)
            ASTUtility::Print(var, Context, "Rule012");
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
    AutoVarPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(autoVarMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
