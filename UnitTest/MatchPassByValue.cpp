#include "ASTUtility.h" 

//match char
DeclarationMatcher funcDeclMatcher = functionDecl(
        findAll(parmVarDecl().bind("parm")),
        isDefinition()).bind("func");

class FuncPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("func");
        const clang::ParmVarDecl *parm = Result.Nodes.getNodeAs<clang::ParmVarDecl>("parm");

        if(!func || !parm || ASTUtility::IsDeclInSTDFile(func, Context)) return;
        
        if (parm -> getType() -> isPointerType() || parm -> getType() -> isArrayType() ||
                parm -> getType() -> isReferenceType()) return;

        //not the parameter is passed by value
        if (!parm -> getType() -> getBaseElementTypeUnsafe() -> isBuiltinType())
            ASTUtility::Print(parm, Context, "Rule038");
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
    FuncPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(funcDeclMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
