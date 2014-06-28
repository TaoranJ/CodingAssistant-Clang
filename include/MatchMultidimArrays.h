#ifndef _MATCH_MULTIDIM_ARRAYS_H
#define _MATCH_MULTIDIM_ARRAYS_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //match type array[]
    extern DeclarationMatcher multiArrayMatcherPL;

    class MultiArrayPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
