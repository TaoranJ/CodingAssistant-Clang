#ifndef _MATCH_VAR_PARM_H
#define _MATCH_VAR_PARM_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    extern StatementMatcher funcVarParmMatcherPL;

    class FunctionVarParmPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result); 
    };
}
#endif
