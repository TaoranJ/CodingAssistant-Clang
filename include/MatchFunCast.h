#ifndef _MATCH_FUN_CAST_H
#define _MATCH_FUN_CAST_H

#include "ASTUtility.h"
namespace ProgrammingLanguage {

    using namespace clang::ast_matchers;
    extern StatementMatcher funcCastMatcherPL;

    class FunctionalCastPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
