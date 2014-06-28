#ifndef _MATCH_NARROWING_CONVERSION_H
#define _MATCH_NARROWING_CONVERSION_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //only a normal implicat cast
    extern StatementMatcher norCastMatcherPL;

    class NarrowCastPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result); 
    };
}
#endif
