#include "ASTUtility.h"
#include <map>

//声明与使用的语句之间距离阀值
const int usedDeclDistance = 2;

StatementMatcher varDefineMatcher = declRefExpr().bind("def");

std::map<const std::string, int>varDefineTest; //<varName, diffLine>

class VarDefineChecker : public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		//get the ASTContext
		clang::ASTContext *Context = Result.Context;
		const clang::DeclRefExpr *def = Result.Nodes.getNodeAs<clang::DeclRefExpr>("def");

		if(!def || ASTUtility::IsStmtInSTDFile(def, Context)) return;

        //only var decl is needed, no field decl or other decl
        if (!def -> getDecl() || !clang::VarDecl::classof(def -> getDecl())) return;
        
        const clang::VarDecl *vd = static_cast<const clang::VarDecl*>(def -> getDecl());
        const std::string varName = ASTUtility::Print(vd, Context);

        //already test
        if (varDefineTest.find(varName) != varDefineTest.end()) return;
        
        auto endLocDecl = Context -> getFullLoc(vd -> getLocEnd());
        auto startLocStmt = Context -> getFullLoc(def -> getLocStart());
        int declLineNo = endLocDecl.isValid() ? endLocDecl.getSpellingLineNumber() : -1; 
        int usedLineNo = startLocStmt.isValid() ? startLocStmt.getSpellingLineNumber() : -1;
        //something is wrong
        if (declLineNo == -1 || usedLineNo == -1) return;

        varDefineTest[varName] = usedLineNo - declLineNo;   
        if (varDefineTest[varName] > usedDeclDistance)
            ASTUtility::Print(vd, Context, "Rule056");
    }		
};

static llvm::cl::OptionCategory MyToolCategory("options");

//Main @TestNeed
int main(int argc, const char **argv){
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(),
					OptionsParser.getSourcePathList());
	VarDefineChecker vdc;
	MatchFinder finder;
	finder.addMatcher(varDefineMatcher,&vdc);
	Tool.run(newFrontendActionFactory(&finder));

    for (auto p : varDefineTest)
    {
        std::cout << p.first << "  :  " << p.second << std::endl;
    }
}

