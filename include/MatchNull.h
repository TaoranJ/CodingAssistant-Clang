#ifndef _MATCH_NULL_H
#define _MATCH_NULL_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    // int *x = NULL or int *x = 0
    extern DeclarationMatcher nullPointerMatcherPL;
    // x == NULL  or x == 0
    extern StatementMatcher biOpMatcher1PL;
    // x != NULL or x != 0
    extern StatementMatcher biOpMatcher2PL;
    // x != NULL or x != 0
    extern StatementMatcher biOpMatcher3PL;

    class NullPointerPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
