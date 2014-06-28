#include "ASTUtility.h" 

//match sturct/union/class 
DeclarationMatcher enumDeclMatcher = enumDecl().bind("enumDecl");

class EnumDeclPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::EnumDecl *enumerator = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl");

        if(!enumerator || ASTUtility::IsDeclInSTDFile(enumerator, Context)) return; 

        if (!enumerator -> isScopedUsingClassTag())
            ASTUtility::Print(enumerator, Context, "Rule025");
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
    EnumDeclPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(enumDeclMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
