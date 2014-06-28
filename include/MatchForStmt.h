#ifndef _MATCH_FOR_STMT_H
#define _MATCH_FOR_STMT_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //match for-range statement 
    extern StatementMatcher forRangeStmtMatcherPL;
    //match for statement
    extern StatementMatcher forStmtMatcherPL;

    class ForRangeStmtPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif

