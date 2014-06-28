#include "ASTUtility.h"
#include <sstream>

//match lambda expr 
StatementMatcher lambdaMatcher = lambdaExpr().bind("lam");

void RemoveWhiteSpace(std::string& str)
{
    while( str.find('\n') != std::string::npos)
    {
        str.erase(str.find('\n'), 1);
    }
    while( str.find(' ') != std::string::npos)
    {
        str.erase(str.find(' '), 1);
    }
}

bool IsLambdaTooLong(const std::string str)
{
    std::istringstream inStrAll(str);
    std::string temp;
    int countLength = 0;
    while( inStrAll >> temp) countLength++;            
    std::ofstream out("Rule032.length", std::ofstream::app);
    if (out)
    {
        out << countLength << "\n";
        out.close();
    }
    if(countLength >= 10) return true;
    return false;
}

bool IsThereADeduction(const std::string& header, const std::size_t& pos)
{
    //identify the ()
    std::istringstream in(header);
    char c = 0;
    int count = 0;
    int bPos = -1;
    std::size_t firstB = header.find('(');
    while(in >> c)
    {
        bPos++;//current pos
        if(0 == count && bPos > firstB) break;
        if( '(' == c) count++;
        if( ')' == c) count--;
    }
    if(bPos <= pos) return true;
    return false; 
}

class LambdaPrinter : public MatchFinder::MatchCallback { public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::LambdaExpr *lam = Result.Nodes.getNodeAs<clang::LambdaExpr>("lam");

        if(!lam || ASTUtility::IsStmtInSTDFile(lam, Context)) return;

        //store lambda
        std::string resultAll;
        llvm::raw_string_ostream strAll(resultAll);
        lam -> printPretty(strAll, nullptr, clang::PrintingPolicy(Context -> getLangOpts()));
        std::string result1 = strAll.str();

        //to judge if lambda is too long
        if (IsLambdaTooLong(result1)) ASTUtility::Print(lam, Context, "Rule032");
        //end
        RemoveWhiteSpace(result1);

        //store lambda body
        std::string resultBody;       
        llvm::raw_string_ostream strBody(resultBody);
        lam -> getBody() -> printPretty(strBody, nullptr, clang::PrintingPolicy(Context -> getLangOpts()));
        std::string result2 = strBody.str();
        RemoveWhiteSpace(result2);
        

        //if there lambda deduce the return type
        std::string header = result1.erase(result1.find(result2));
        std::size_t pos = header.rfind("->");
        if(std::string::npos == pos || header.find('(') == std::string::npos) return;

        if (!IsThereADeduction(header, pos)) 
            ASTUtility::Print(lam, Context, "Rule033");
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
    LambdaPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(lambdaMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
