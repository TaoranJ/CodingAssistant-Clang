#include "ASTUtility.h" 

//match sturct/union/class 
DeclarationMatcher recordDeclMatcher = recordDecl(has(fieldDecl())).bind("recordDecl");

class RecordDeclPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::RecordDecl *st = Result.Nodes.getNodeAs<clang::RecordDecl>("recordDecl");
        std::vector<std::string> typeList;

        //only need struct
        if(!st || !st -> isCompleteDefinition() || !st -> isStruct() || ASTUtility::IsDeclInSTDFile(st, Context)) return;

        for(auto it = st -> field_begin(); it != st -> field_end(); it++)
            if((*it) -> isBitField())
                ASTUtility::Print(*it, Context, "Rule024");
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
    RecordDeclPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(recordDeclMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
