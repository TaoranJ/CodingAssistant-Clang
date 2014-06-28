#ifndef _MATCH_NEW_EXPR_H
#define _MATCH_NEW_EXPR_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {

    using namespace clang::ast_matchers;
    //
    extern StatementMatcher cxxNewExprMatcherPL;

    class NewExprPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result); 
    };
}
#endif
