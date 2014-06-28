#include "ASTUtility.h" 

//match void*
DeclarationMatcher voidPointerMatcher = declaratorDecl(hasType(pointsTo(builtinType().bind("type")))).bind("var");
//match void**
DeclarationMatcher voidPtrPtrMatcher = declaratorDecl(hasType(pointsTo(
                    pointsTo(builtinType().bind("typePP"))))).bind("varPP");
//match void* []
DeclarationMatcher voidPtrArrayMatcher = declaratorDecl(hasType(arrayType(
                hasElementType(pointsTo(builtinType().bind("typeAry")))))).bind("varAry");

class VoidPointerPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::BuiltinType *type = Result.Nodes.getNodeAs<clang::BuiltinType>("type");
        const clang::DeclaratorDecl *var = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("var");
        const clang::DeclaratorDecl *varAry = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("varAry");
        const clang::BuiltinType *typeAry = Result.Nodes.getNodeAs<clang::BuiltinType>("typeAry");
        const clang::DeclaratorDecl *varPP = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("varPP");
        const clang::BuiltinType *typePP = Result.Nodes.getNodeAs<clang::BuiltinType>("typePP");

        if(type && var && !ASTUtility::IsDeclInSTDFile(var, Context) && type -> isVoidType())
            ASTUtility::Print(var, Context, "Rule022");
        
        if (varAry && typeAry && !ASTUtility::IsDeclInSTDFile(varAry, Context) && typeAry -> isVoidType())
            ASTUtility::Print(varAry, Context, "Rule022"); 

        if (varPP && typePP && !ASTUtility::IsDeclInSTDFile(varPP, Context) && typePP -> isVoidType())
            ASTUtility::Print(varPP, Context, "Rule022"); 
    }
};

//"My tool options" is the name.
static llvm::cl::OptionCategory MyToolCategory("My tool options");

int main(int argc, const char **argv)
{
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    VoidPointerPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(voidPointerMatcher, &Printer);
    Finder.addMatcher(voidPtrPtrMatcher, &Printer);
    Finder.addMatcher(voidPtrArrayMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));
    return 0;
}
