#include "ASTUtility.h"

//match namespace 
DeclarationMatcher namespaceMatcher = namespaceDecl().bind("namespaceDecl");


class NamespacePrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::NamespaceDecl *scope = Result.Nodes.getNodeAs<clang::NamespaceDecl>("namespaceDecl");

        if(!scope || ASTUtility::IsDeclInSTDFile(scope, Context)) return;
        
        std::string name = scope -> getNameAsString();
        
        //short: <= 3 but not the anonymous namespace
        if (name.length() <= 3 && name.length() != 0)
            ASTUtility::Print(scope, Context, "Rule043");
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
    NamespacePrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(namespaceMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
