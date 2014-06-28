#ifndef _MATCH_UNINIT_VAR_H
#define _MATCH_UNINIT_VAR_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    extern DeclarationMatcher uninitializedVarMatcherPL;

    class UninitVarPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
