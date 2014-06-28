#include "../include/MatchOverloading.h"

namespace ProgrammingLanguage {
    StatementMatcher allFuncMatcherPL = compoundStmt(
            hasParent(functionDecl(
                    parameterCountIs(1),
                    hasAnyParameter(hasType(isInteger()))).bind("function"))
            ).bind("functionBody");

    std::map<std::string, int> countOverloding;
}

void ProgrammingLanguage::OverloadingFuncPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::CompoundStmt *funBody = Result.Nodes.getNodeAs<clang::CompoundStmt>("functionBody");
    const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("function");

    if(!func || ASTUtility::IsDeclInSTDFile(func, Context) || func -> isVariadic()) return;

    std::string str;
    llvm::raw_string_ostream rss(str);
    func -> getNameForDiagnostic(rss, clang::PrintingPolicy(Context -> getLangOpts()), true);

    std::string name = rss.str();

    //overload count
    if(countOverloding.end() != countOverloding.find(name))
        countOverloding[name]++;
    else
        countOverloding[name] = 1;
}
