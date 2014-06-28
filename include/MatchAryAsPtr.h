#ifndef _MATCH_ARY_AS_PTR_H
#define _MATCH_ARY_AS_PTR_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    extern StatementMatcher simpleImpCastMatcherPL;
    
    class ArrayAsPtrPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result); 
    };
}
#endif
