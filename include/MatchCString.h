#ifndef _MATCH_CSTRING_H
#define _MATCH_CSTRING_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {

    using namespace clang::ast_matchers;
    //a means array, p means pointer
    //match char*
    extern DeclarationMatcher charPointerMatcherPL;

    //match char[]
    extern DeclarationMatcher charArrayMatcherPL;

    //match char *array[]
    extern DeclarationMatcher arrayOfCharPointerMatcherPL;

    //match char **array
    extern DeclarationMatcher charPtrPtrMatcherPL;

    class CharPointerPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
