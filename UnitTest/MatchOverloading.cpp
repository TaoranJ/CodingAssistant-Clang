#include "ASTUtility.h"

StatementMatcher funcMatcher = compoundStmt(
        hasParent(functionDecl(
                parameterCountIs(1),
                hasAnyParameter(hasType(isInteger()))).bind("function"))
        ).bind("functionBody");

static std::map<std::string, int> count;
class FunctionPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::CompoundStmt *funBody = Result.Nodes.getNodeAs<clang::CompoundStmt>("functionBody");
        const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("function");

        if(!func || ASTUtility::IsDeclInSTDFile(func, Context) || func -> isVariadic()) return;

        std::string str;
        llvm::raw_string_ostream rss(str);
        func -> getNameForDiagnostic(rss, clang::PrintingPolicy(Context -> getLangOpts()), true);

        std::string name = rss.str();

        //overload count
        if(count.end() != count.find(name))
            count[name]++;
        else
            count[name] = 1;
    }
};

//"My tool options" is the name.
static llvm::cl::OptionCategory MyToolCategory("My tool options");

int main(int argc, const char **argv)
{
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    FunctionPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(funcMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));

    for(auto it : count)
    {
        if (it.second >= 2 && it.second <= 4)
        {
            std::ofstream out("Rule042", std::ofstream::app); 
            out << it.first << " : " << it.second << '\n';
            out.close();
        }
    }
    return 0;
}
