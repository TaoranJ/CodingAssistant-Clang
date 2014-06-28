#ifndef _MATCH_CMEM_FUNC_H
#define _MATCH_CMEM_FUNC_H

#include "ASTUtility.h"

namespace CodStd101 {
    using namespace clang::ast_matchers;

    //StatementMatcher callExprMatcher = callExpr().bind("callexpr");
    extern StatementMatcher mallocMatcherCS; 
    extern StatementMatcher freeMatcherCS; 
    extern StatementMatcher reallocMatcherCS;
    extern StatementMatcher memcpyAryMatcherCS;
    extern StatementMatcher memcpyPtrMatcherCS;
    extern StatementMatcher memcmpAryMatcherCS;
    extern StatementMatcher memcmpPtrMatcherCS;

    class CallExprPrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
