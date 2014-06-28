#ifndef _MATCH_VIRTUAL_CALLEE_H
#define _MATCH_VIRTUAL_CALLEE_H

#include "ASTUtility.h"

namespace EffectiveCPP {
    using namespace clang::ast_matchers;
    extern StatementMatcher ctorCallVtlMatcherEC;
    extern StatementMatcher dtorCallVtlMatcherEC;

    class CtorCallVtlPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
