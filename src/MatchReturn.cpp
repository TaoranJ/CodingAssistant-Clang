#include "ASTUtility.h"

std::vector<std::string> localVarVec;
StatementMatcher funcReturntMatcher = functionDecl(
        returns(pointerType()),
        ).bind("func");

void parseStmt(const clang::Stmt *stmt){
	std::vector<std::string> varNames;
	if(clang::DeclStmt::classof(stmt)){
		clang::DeclStmt *ds = static_cast<clang::DeclStmt *>(const_cast<clang::Stmt *>(stmt));
		for(clang::DeclStmt::const_decl_iterator cdi = ds->decl_begin(); cdi != ds->decl_end(); ++cdi){
			if(clang::ValueDecl::classof(*cdi)){
				const clang::ValueDecl *vd = static_cast<clang::ValueDecl *>(const_cast<clang::Decl *>(*cdi));
				clang::QualType qt(vd->getType());
				const clang::Type* tp = qt.getTypePtr();
				if(tp->isPointerType() == true || tp->isReferenceType() == true){
					localVarVec.push_back(vd->getNameAsString());
				}
			}
		}
	}
	if(clang::BinaryOperator::classof(stmt)){
		clang::BinaryOperator *bop = static_cast<clang::BinaryOperator *>(const_cast<clang::Stmt *>(stmt));
		clang::Expr *expr = bop->getLHS();
		if(clang::DeclRefExpr::classof(expr)){
			const clang::DeclRefExpr *dre = static_cast<clang::DeclRefExpr *>(const_cast<clang::Expr *>(expr));
			const clang::ValueDecl *vd = dre->getDecl();
			clang::QualType qt(vd->getType());
			const clang::Type* tp = qt.getTypePtr();
			if(tp->isPointerType() == true || tp->isReferenceType() == true){
				localVarVec.push_back(vd->getNameAsString());
			}
		}
	}	
}

class ReturnPrinter :  public MatchFinder::MatchCallback {
public:
	virtual void run(const MatchFinder::MatchResult &Result)
    {
		clang::ASTContext *Context = Result.Context;
		const clang::CompoundStmt *cs = Result.Nodes.getNodeAs<clang::CompoundStmt>("funcstmt");
		const clang::FunctionDecl *fd = Result.Nodes.getNodeAs<clang::FunctionDecl>("functiondecl");

		std::string funcname = fd->getNameAsString();
		if(!cs || !fd || ASTUtility::IsStmtInSTDFile(cs, Context))
			return;
		for(clang::CompoundStmt::const_body_iterator cdi = cs->body_begin(); cdi != cs->body_end(); ++cdi){
			parseStmt(*cdi);
			//方法作用域的ReturnStmt后面肯定不会有变量声明
			if(clang::ReturnStmt::classof(*cdi)){
				const clang::ReturnStmt *rs = static_cast<clang::ReturnStmt *>(const_cast<clang::Stmt *>(*cdi));
				const clang::Expr *expr = rs->getRetValue();
				std::string str;
				llvm::raw_string_ostream rso(str);
				expr->printPretty(rso, 0, clang::PrintingPolicy(Context -> getLangOpts()));
				std::string returnVar(rso.str());
				for(int i = 0; i < localVarVec.size(); ++i){
					if(returnVar.compare(localVarVec[i]) == 0)
						ASTUtility::Print(rs, Context, "Rule66");		
				}
			}
		}
		return;
	}
};
