#ifndef _MATCH_REF_PARM_H
#define _MATCH_REF_PARM_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //match a simple funcion including method
    extern DeclarationMatcher funcMatcherPL;
    extern int countRef;

    class ParmPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result); 
    };
}

#endif
