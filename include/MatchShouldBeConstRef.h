#ifndef _MATCH_SHOULD_BE_CONST_REF_H
#define _MATCH_SHOULD_BE_CONST_REF_H

#include "ASTUtility.h" 
#include <map>
#include <vector>
#include <string>

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    extern DeclarationMatcher funcRefParmMatcherPL;
    extern std::map< std::string, std::vector<std::string> >funcParm; //<funcName, non-const-ref parm vector>
    extern std::map< std::string, std::vector<std::string> >funcParmUsed;
    void LoopThroughCS(const clang::Stmt* funcBody, const clang::ASTContext* Context, const std::string func);

    class FuncRefParmPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
