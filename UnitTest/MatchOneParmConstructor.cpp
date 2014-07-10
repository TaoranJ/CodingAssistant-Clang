#include "ASTUtility.h"


DeclarationMatcher classOneParmConstructorMatcherPL = recordDecl(
            isDefinition(),
            findAll(constructorDecl(parameterCountIs(1)).bind("oneParmConstructor"))
            ).bind("targetClass");

class OneParmConstructorPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result)
        {
	        clang::ASTContext *Context = Result.Context;
            const clang::CXXRecordDecl *targetClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("targetClass");
            const clang::CXXConstructorDecl *targetConstructor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("oneParmConstructor");

            if (!targetClass || ASTUtility::IsDeclInSTDFile(targetClass, Context) ||
                    !targetConstructor || ASTUtility::IsDeclInSTDFile(targetConstructor, Context))
                return;

            if (!clang::CXXConstructorDecl::classof(targetConstructor)) return; 
            //we don't care copy or move constructor
            if (targetConstructor -> isCopyConstructor() || targetConstructor -> isMoveConstructor())
                return;
            //ok, find explicit keyword
            if (targetConstructor -> isExplicit()) 
                return;
            ASTUtility::Print(targetConstructor, Context, "Rule066");
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
    OneParmConstructorPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(classOneParmConstructorMatcherPL, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
