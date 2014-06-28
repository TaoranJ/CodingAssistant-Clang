#ifndef _MATCH_MAIN_CATCH_H
#define _MATCH_MAIN_CATCH_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {

    using namespace clang::ast_matchers;

    //find func with try catch in it
    extern StatementMatcher tryInFuncMatcherPL;

    class TryInFuncPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
