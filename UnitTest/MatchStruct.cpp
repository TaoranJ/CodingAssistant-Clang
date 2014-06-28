#include "ASTUtility.h" 

//match sturct/union/class 
DeclarationMatcher recordDeclMatcher = recordDecl(has(fieldDecl())).bind("recordDecl");

bool TypeSize(const std::vector<std::string>& typeList)
{
    std::vector<int> typeSize;

    for (auto i : typeList)
    {
        if(i == "char" || i == "signed char" || i == "unsigned char")
            typeSize.push_back(1);
        else if (i == "short" || i == "unsigned short" || i == "char16_t")
            typeSize.push_back(2);
        else if(i == "int" || i == "unsigned int" || i == "float" || i == "char32_t" || i == "pointer")
            typeSize.push_back(4);
        else if(i == "long" || i == "unsigned long")
            typeSize.push_back(5);
        else if(i == "long long" || i == "unsigned long long" || i == "double")
            typeSize.push_back(8);
        else if(i == "long double")
            typeSize.push_back(12);
        else if(i == "__int128" || i == "unsigned __int128")
            typeSize.push_back(16);
        else 
            typeSize.push_back(0);
    }

    for(auto i : typeSize)
        for(auto j : typeSize)
            if(i < j && i != 0) 
                return false;
    return true;    
}

static int count = 0;
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

        count++;
        std::ofstream out("Rule023.back");
        if (!out) return; 
        out << count;

        for(clang::RecordDecl::field_iterator it = st -> field_begin(); it != st -> field_end(); it++)
        {
            if((*it) -> getType() -> isBuiltinType())
            {
                typeList.push_back(clang::dyn_cast<clang::BuiltinType>((*it) -> getType()) 
                        -> getName(clang::PrintingPolicy(Context -> getLangOpts())).str());
            }
            else if((*it) -> getType() -> isPointerType())
            {
                typeList.push_back(std::string("pointer"));
            }
            else
            {
                typeList.push_back(std::string("nothing"));
            }
        }

        if (!TypeSize(typeList))
            ASTUtility::Print(st, Context, "Rule023");
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
