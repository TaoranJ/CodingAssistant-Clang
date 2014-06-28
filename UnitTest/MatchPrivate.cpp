#include "ASTUtility.h"

DeclarationMatcher recordDeclMatcher = recordDecl(eachOf(
            has(fieldDecl(isPublic()).bind("pub")),
            has(fieldDecl(isProtected()).bind("pro"))
            )).bind("record");

class RecordPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result)
    {
		clang::ASTContext *Context = Result.Context;
		const clang::CXXRecordDecl *record = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record");
		const clang::FieldDecl *pub = Result.Nodes.getNodeAs<clang::FieldDecl>("pub");
		const clang::FieldDecl *pro = Result.Nodes.getNodeAs<clang::FieldDecl>("pro");

        if (!record || ASTUtility::IsDeclInSTDFile(record, Context) || !record -> getDefinition()) return;
   
        //or may be isPOD(), or maybe isCLike()
        if (record -> isAggregate()) return; 

        ASTUtility::Print(record, Context, "Rule059.back");
                  
        if (pub) ASTUtility::Print(pub, Context, "Rule059");
        if (pro) ASTUtility::Print(pro, Context, "Rule059");
    }
};

//@TestNeed
static llvm::cl::OptionCategory MyToolCategory("options");

/*
 * Main @TestNeed
 */
int main(int argc, const char **argv){
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	//Create an ClangTool instance to run a FrontendAction over a set of files
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	//tool::MyFactory Factory;
	RecordPrinter fc;
	MatchFinder finder;
	finder.addMatcher(recordDeclMatcher, &fc);
	
	Tool.run(newFrontendActionFactory(&finder));
	return 0;
}
