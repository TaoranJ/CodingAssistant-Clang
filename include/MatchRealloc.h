#ifndef _MATCH_REALLOC_H
#define _MATCH_REALLOC_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //find func realloc 
    extern StatementMatcher callExprMatcherPL;
    
    class CallExprPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
