#ifndef _MATCH_GOTO_STMT_H
#define _MATCH_GOTO_STMT_H

#include "ASTUtility.h" 
namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //match do statement
    extern StatementMatcher gotoStmtMatcherPL;
    
    class GotoStmtPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
