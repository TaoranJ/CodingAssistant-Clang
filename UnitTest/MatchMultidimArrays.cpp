#include "ASTUtility.h"

//match type array[]
DeclarationMatcher arrayMatcher = declaratorDecl(hasType(arrayType(hasElementType(arrayType())))).bind("varArray");

class ArrayPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::DeclaratorDecl *varArray = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("varArray");

        if(!varArray || ASTUtility::IsDeclInSTDFile(varArray, Context)) return;

        ASTUtility::Print(varArray, Context, "Rule016");
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
    ArrayPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(arrayMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
