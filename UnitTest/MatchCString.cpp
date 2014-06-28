#include "ASTUtility.h" 

//a means array, p means pointer
//match char*
DeclarationMatcher charPointerMatcher = declaratorDecl(hasType(pointsTo(
                builtinType().bind("pType")))).bind("pVar");
//match char[]
DeclarationMatcher charArrayMatcher = declaratorDecl(hasType(arrayType(
                hasElementType(builtinType().bind("aType"))))).bind("aVar");
//match char *array[]
DeclarationMatcher arrayOfCharPointerMatcher = declaratorDecl(hasType(arrayType(
                hasElementType(pointerType(pointee(builtinType().bind("apType"))))))
        ).bind("apVar");
//match char **array
DeclarationMatcher charPtrPtrMatcher = declaratorDecl(hasType(pointsTo(
                pointerType(pointee(builtinType().bind("ppType")))))
        ).bind("ppVar");

class CharPointerPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        //charPointerMatcher
        const clang::BuiltinType *pType = Result.Nodes.getNodeAs<clang::BuiltinType>("pType");
        const clang::DeclaratorDecl *pVar = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("pVar");
        //charArrayMatcher
        const clang::BuiltinType *aType = Result.Nodes.getNodeAs<clang::BuiltinType>("aType");
        const clang::DeclaratorDecl *aVar = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("aVar");
        //arrayOfCharPointerMatcher
        const clang::BuiltinType *apType = Result.Nodes.getNodeAs<clang::BuiltinType>("apType");
        const clang::DeclaratorDecl *apVar = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("apVar");
        //charPtrPtrMatcher
        const clang::BuiltinType *ppType = Result.Nodes.getNodeAs<clang::BuiltinType>("ppType");
        const clang::DeclaratorDecl *ppVar = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("ppVar");


        if (pType && pVar && !ASTUtility::IsDeclInSTDFile(pVar, Context) && pType -> isAnyCharacterType())
            ASTUtility::Print(pVar, Context, "Rule001");
        else if (aType && aVar && !ASTUtility::IsDeclInSTDFile(aVar, Context) && aType -> isAnyCharacterType())
            ASTUtility::Print(aVar, Context, "Rule001");
        else if (apType && apVar && !ASTUtility::IsDeclInSTDFile(apVar, Context) && apType -> isAnyCharacterType())
            ASTUtility::Print(apVar, Context, "Rule001");
        else if (ppType && ppVar && !ASTUtility::IsDeclInSTDFile(ppVar, Context) && ppType -> isAnyCharacterType())
            ASTUtility::Print(ppVar, Context, "Rule001");
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
    CharPointerPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(charPointerMatcher, &Printer);
    Finder.addMatcher(charArrayMatcher, &Printer);
    Finder.addMatcher(arrayOfCharPointerMatcher, &Printer);
    Finder.addMatcher(charPtrPtrMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
