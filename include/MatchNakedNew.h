#ifndef _MATCH_NAKED_NEW_H
#define _MATCH_NAKED_NEW_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //match naked new 
    extern DeclarationMatcher nakedNewMatcherPL;
    class NakedNewPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
