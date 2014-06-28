#include "ASTUtility.h"

//function
StatementMatcher returnMatcher = compoundStmt(
        hasParent(functionDecl().bind("func")),
        findAll(returnStmt().bind("stmt")));

class ReturnPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("func");
        const clang::ReturnStmt *retStmt = Result.Nodes.getNodeAs<clang::ReturnStmt>("stmt");

        if(!func || !retStmt || ASTUtility::IsDeclInSTDFile(func, Context)) return;

        //overloader operator
        if( func -> isOverloadedOperator() ) return;

        if (func -> getReturnType() -> isPointerType() || func -> getReturnType() -> isReferenceType())
            ASTUtility::Print(func, Context, "Rule039");
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
    ReturnPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(returnMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
