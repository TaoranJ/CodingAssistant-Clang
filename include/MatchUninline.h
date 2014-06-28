#ifndef _MATCH_UN_INLINE_H
#define _MATCH_UN_INLINE_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    extern DeclarationMatcher functionDeclMatcherPL;

    class UnInlineFuncPrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
