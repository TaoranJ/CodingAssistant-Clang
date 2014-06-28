#include "../include/MatchVarDefine.h"

namespace EffectiveCPP {
    const int usedDeclDistance = 2;
    StatementMatcher varUseMatcherEC = declRefExpr().bind("def");
    std::map<const std::string, int>varDefineTest; //<varName, diffLine>
}

void EffectiveCPP::VarDefinePrinter::run(const MatchFinder::MatchResult &Result)
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
