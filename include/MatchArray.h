#ifndef _MATCH_ARRAY_H
#define _MATCH_ARRAY_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    
    //match built-in array[]
    extern DeclarationMatcher arrayMatcherPL;

    class ArrayPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}

#endif
