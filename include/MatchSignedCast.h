#ifndef _MATCH_SIGNED_CAST_H
#define _MATCH_SIGNED_CAST_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //only match implicit cast
    extern StatementMatcher impCastExprMatcherPL;

    class ImpCastExprPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
