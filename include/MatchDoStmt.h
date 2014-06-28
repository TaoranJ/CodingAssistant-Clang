#ifndef _MATCH_DO_STMT_H
#define _MATCH_DO_STMT_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //match do statement
    extern StatementMatcher doStmtMatcherPL;

    class DoStmtPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
