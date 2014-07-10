#ifndef _MATCH_ONE_PARM_CONSTRUCTOR_H
#define _MATCH_ONE_PARM_CONSTRUCTOR_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //match constructor with one parmeter
    extern DeclarationMatcher classOneParmConstructorMatcherPL;

    class OneParmConstructorPrinter : public MatchFinder::MatchCallback {
        public:
            virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
