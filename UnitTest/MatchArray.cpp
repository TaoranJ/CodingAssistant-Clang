#include "ASTUtility.h" 

//match built-in array[]
DeclarationMatcher arrayMatcher = declaratorDecl(hasType(arrayType())).bind("var");

class ArrayPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        clang::ASTContext* Context = Result.Context;
        //get the node
        const clang::DeclaratorDecl *var = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("var");

        //if not valid, return
        if(!var || ASTUtility::IsDeclInSTDFile(var, Context)) return;

        //invalid means user-defined type, when we can't find right include
        //files.
        if (var -> isInvalidDecl())
        {
            ASTUtility::Print(var, Context, "Rule003");
        }
        else
        {
            if(var -> getTypeSourceInfo() -> getType() -> getBaseElementTypeUnsafe() -> isBuiltinType())
                ASTUtility::Print(var, Context, "Rule002");
            else
                ASTUtility::Print(var, Context, "Rule003");
        }
    }
};

//"My tool options" is the name.
static llvm::cl::OptionCategory MyToolCategory("My tool options");

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
    ArrayPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(arrayMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
