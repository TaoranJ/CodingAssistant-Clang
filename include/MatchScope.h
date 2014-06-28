#ifndef _MATCH_SCOPE_H
#define _MATCH_SCOPE_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {

    using namespace clang::ast_matchers;
    extern const int scopeThreshold;

    //I'm not sure. Function has been checked, so no FunctionDecl
    //here find only the leaf scope in which there is no scope anymore.
    extern StatementMatcher scopeStmtMatcherPL;
    class ScopeLinesPrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
} 
#endif

