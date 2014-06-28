#ifndef _MATCH_FUNC_CONSTEXPR_H
#define _MATCH_FUNC_CONSTEXPR_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    extern StatementMatcher normalFuncMatcherPL;

    class FunctionPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
