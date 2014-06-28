#include "ASTUtility.h" 

StatementMatcher impCastExprMatcher = implicitCastExpr().bind("cast");

class ImpCastExprPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::ImplicitCastExpr *cast = Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("cast");

        if(!cast || ASTUtility::IsStmtInSTDFile(cast, Context)) return;

        std::string left = cast -> getType().getAsString();
        std::string right = cast -> getSubExpr() -> getType().getAsString();

        //unsigned ... = signed... OR signed(or null) ... = unsigned ...
        if ((left.find("unsigned ") != std::string::npos && right.find("unsigned ") == std::string::npos) ||
                (left.find("unsigned ") == std::string::npos && right.find("unsigned ") != std::string::npos))
            ASTUtility::Print(cast, Context, "Rule009");
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
    ImpCastExprPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(impCastExprMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
