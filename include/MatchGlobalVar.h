#ifndef _MATCH_GLOBAL_VAR_H
#define _MATCH_GLOBAL_VAR_H

#include "ASTUtility.h"

namespace CodStd101 {
    using namespace clang::ast_matchers;

    extern const int globalVarLength;
    extern DeclarationMatcher normalVarDeclMatcherCS;
    
    class GlobalVarPrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
