#ifndef _MATCH_FUNC_LINES_H
#define _MATCH_FUNC_LINES_H

#include "ASTUtility.h"

namespace CodStd101 {
    using namespace clang::ast_matchers;

    //threshold value , it can be changed in configure file
    extern const int funcThreshold;

    extern StatementMatcher funcStmtMatcherCS;
    class FuncLinesPrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif

