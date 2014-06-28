#include "ASTUtility.h" 

//match function here
StatementMatcher callExprMatcher = callExpr(callee(functionDecl().bind("funcDecl"))).bind("callExpr");
//match var declarations here
DeclarationMatcher varMatcher = varDecl().bind("var");

class StdPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::CallExpr *call = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr");
        const clang::FunctionDecl *fun = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl");
        const clang::VarDecl *var = Result.Nodes.getNodeAs<clang::VarDecl>("var");

        if(call && !ASTUtility::IsStmtInSTDFile(call, Context))
        {
            if (ASTUtility::IsDeclInSTDFile(fun, Context)) 
                ASTUtility::Print(fun, Context, "Rule029.stmt.std");
            else
                ASTUtility::Print(fun, Context, "Rule029.stmt.usr");
        }

        if (var && !ASTUtility::IsDeclInSTDFile(var, Context))
        {
            if (var -> getType().getAsString().find("std::") != std::string::npos)
                ASTUtility::Print(var, Context, "Rule029.decl.std");
            else
                ASTUtility::Print(var, Context, "Rule029.decl.usr");
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
    StdPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(callExprMatcher, &Printer);
    Finder.addMatcher(varMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
