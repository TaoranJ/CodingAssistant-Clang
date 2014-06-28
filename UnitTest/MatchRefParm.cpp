#include "ASTUtility.h" 

DeclarationMatcher funcMatcher = functionDecl().bind("func");

static int count = 0;
class ParmPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("func");

        if(!func || !func -> hasBody() || ASTUtility::IsDeclInSTDFile(func, Context)) return;
        
        for(auto p : func -> parameters()) 
        {
            if (p -> getType() -> isReferenceType())
            {
                count++;
                std::ofstream out("Rule018.back");
                if (!out) return; 
                out << count;
                if (p -> getType().getNonReferenceType().isConstQualified()) continue;
                ASTUtility::Print(p, Context, "Rule018");
            }
        }
    }
};

//"My tool options" is the name.
static llvm::cl::OptionCategory MyToolCategory("My tool options");

int main(int argc, const char **argv)
{
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    //tooling::MyFactory Factory;
    ParmPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(funcMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
