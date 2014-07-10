#include "ASTUtility.h"


DeclarationMatcher classOnlyDefaultConstructorMatcherPL = recordDecl(
            isDefinition(),
            unless(has(constructorDecl(hasAnyParameter(anything()))))).bind("targetClass");

class OnlyDefaultConstructorClassPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result)
        {
	        clang::ASTContext *Context = Result.Context;
            const clang::CXXRecordDecl *targetClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("targetClass");

            if (!targetClass || ASTUtility::IsDeclInSTDFile(targetClass, Context))
                return;

            ASTUtility::Print(targetClass, Context, "Rule065");
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
    OnlyDefaultConstructorClassPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(classOnlyDefaultConstructorMatcherPL, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
