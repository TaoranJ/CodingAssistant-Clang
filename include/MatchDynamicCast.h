#ifndef _MATCH_DYNAMIC_CAST_H
#define _MATCH_DYNAMIC_CAST_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    extern StatementMatcher dynamicCastMatcherPL;

    class DynamicCastPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
