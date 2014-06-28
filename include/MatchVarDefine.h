#ifndef _MATCH_VAR_DEFINE_H
#define _MATCH_VAR_DEFINE_H

#include "ASTUtility.h"
#include <map>

namespace EffectiveCPP {
    using namespace clang::ast_matchers;
    
    extern StatementMatcher varUseMatcherEC;
    extern std::map<const std::string, int>varDefineTest; //<varName, diffLine>
    extern const int usedDeclDistance;
    
    class VarDefinePrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
} 
#endif
