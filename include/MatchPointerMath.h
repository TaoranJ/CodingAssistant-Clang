#ifndef _MATCH_POINTER_MATCH_H
#define _MATCH_POINTER_MATCH_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    extern StatementMatcher binaryOperatorMatcherPL;
    
    class PtrMemOpPrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
