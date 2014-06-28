#include "ASTUtility.h" 

StatementMatcher callMatcher = operatorCallExpr(hasOverloadedOperatorName("[]")).bind("op");

class CallPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::CXXOperatorCallExpr *COCE = Result.Nodes.getNodeAs<clang::CXXOperatorCallExpr>("op");

        //if not null
        if(!COCE || ASTUtility::IsStmtInSTDFile(COCE, Context)) return;
        
        //if it's not a class method, or its caller has nothing to do with the std
        //container return
        if (!clang::CXXMethodDecl::classof(COCE -> getCalleeDecl())) return;

        const clang::CXXMethodDecl *CMD = static_cast<const clang::CXXMethodDecl *>(COCE -> getCalleeDecl());
        if (!ASTUtility::IsSTDContainer(CMD -> getParent())) return;

        //if the callee is in std file, without range-check 
        if (ASTUtility::IsDeclInSTDFile(COCE -> getCalleeDecl(), Context))
            ASTUtility::Print(COCE, Context, "Rule005");
        else
            ASTUtility::Print(COCE, Context, "Rule004");
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
    CallPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(callMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
