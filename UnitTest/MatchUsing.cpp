#include "ASTUtility.h"

static inline bool checkHeader(std::string fileName) 
{
	if(fileName.find(".hh") == fileName.size() - 3 ||
		fileName.find(".h") == fileName.size() - 2 ||
        fileName.find(".H") == fileName.size() - 2 ||
        fileName.find(".hpp") == fileName.size() - 4)
		return true;
	return false;
}
//only using std:vector
//no using namespace std;
DeclarationMatcher usingDeclMatcher = usingDecl().bind("usingDecl");

class UsingChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		clang::ASTContext *Context = Result.Context;
		const clang::UsingDecl *ud = Result.Nodes.getNodeAs<clang::UsingDecl>("usingDecl");

		if(!ud || ASTUtility::IsDeclInSTDFile(ud, Context)) return;
        std::string loc = ud -> getUsingLoc().printToString(Context -> getSourceManager());

        if (!ud -> getQualifier() -> getAsNamespace() -> getOriginalNamespace() -> getNameAsString().compare("std"))
            return;
        if (!checkHeader(loc.substr(0, loc.find(":")))) return;

		ASTUtility::Print(ud, Context, "Rule68");
	}
};

static llvm::cl::OptionCategory MyToolCategory("options");

//main @TestNeed
int main(int argc, const char **argv) {
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);

	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	UsingChecker uc;
	MatchFinder finder;
	finder.addMatcher(usingDeclMatcher, &uc);
	Tool.run(newFrontendActionFactory(&finder));
	return 0;

}
