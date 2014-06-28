#include "ASTUtility.h"

StatementMatcher funcMatcher = compoundStmt(
        hasParent(functionDecl().bind("function"))         
        ).bind("functionBody");


class FunctionPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::CompoundStmt *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("functionBody");
        const clang::FunctionDecl *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("function");

        if(!FD || ASTUtility::IsDeclInSTDFile(FD, Context)) return;
        
        if(FD -> isVariadic())
            ASTUtility::Print(FD, Context, "Rule041");
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
    FunctionPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(funcMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
