#ifndef _MATCH_AUTO_H
#define _MATCH_AUTO_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //only match var, if auto is tested in the AutoVarPrinter
    extern DeclarationMatcher autoVarMatcherPL;

    class AutoVarPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
