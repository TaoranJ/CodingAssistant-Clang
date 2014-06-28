#include "../include/MatchPrivate.h"

namespace EffectiveCPP {
    DeclarationMatcher recordDeclMatcherEC = recordDecl(eachOf(
            has(fieldDecl(isPublic()).bind("pub")),
            has(fieldDecl(isProtected()).bind("pro"))
            )).bind("record");
}

void EffectiveCPP::RecordPrinter::run(const MatchFinder::MatchResult &Result)
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
