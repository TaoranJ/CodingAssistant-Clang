#ifndef _MATCH_PASS_BY_VALUE_H
#define _MATCH_PASS_BY_VALUE_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //match char
    extern DeclarationMatcher funcParmMatcherPL;

    class FuncParmPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
