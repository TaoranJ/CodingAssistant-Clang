#include "ASTUtility.h"

StatementMatcher impCastMatcher = implicitCastExpr().bind("impCast");

class ImpCastPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::ImplicitCastExpr *cast = Result.Nodes.getNodeAs<clang::ImplicitCastExpr>("impCast");

        if(!cast || ASTUtility::IsStmtInSTDFile(cast, Context)) return;
        
        std::string castType(cast -> getCastKindName());
        if(castType == "DerivedToBase" || castType == "UncheckedDerivedToBase" 
           || castType == "FloatingToIntegral" || castType == "FloatingToBoolean" 
           || castType == "IntegralToBoolean" || castType ==  "ToVoid")
        {
            ASTUtility::Print(cast, Context, "Rule030");
        }
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
    ImpCastPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(impCastMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
