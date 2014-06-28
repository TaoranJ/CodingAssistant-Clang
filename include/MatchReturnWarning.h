#ifndef _MATCH_RETURN_WARNING_H
#define _MATCH_RETURN_WARNING_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //return stmt
    extern StatementMatcher returnMatcherPL;
    class ReturnPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
