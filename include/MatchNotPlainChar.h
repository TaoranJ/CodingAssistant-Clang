#ifndef _MATCH_NOT_PLAIN_CHAR_H
#define _MATCH_NOT_PLAIN_CHAR_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //match only normal declaration 
    extern DeclarationMatcher notPlainCharMatcherPL;

    class NotPlainCharPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
