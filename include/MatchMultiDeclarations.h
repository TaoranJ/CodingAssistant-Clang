#ifndef _MATCH_MULTI_DECLARATIONS_H
#define _MATCH_MULTI_DECLARATIONS_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //match decl stmt
    extern StatementMatcher declStmtMatcherPL;

    class MultiDeclPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
