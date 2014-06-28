#ifndef _MATCH_VIRTUAL_FUNC_H
#define _MATCH_VIRTUAL_FUNC_H

#include "ASTUtility.h"

namespace CodStd101 {
    using namespace clang::ast_matchers;

    extern DeclarationMatcher pubVtlFuncMatcherCS;

    class PubVtlPrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif

