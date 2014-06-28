#ifndef _MATCH_INTEGER_TO_CHAR_H
#define _MATCH_INTEGER_TO_CHAR_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //match  {int x} = 'c';
    extern DeclarationMatcher charLiteralToIntMatcherPL;
    //match {char} c = 12;
    extern DeclarationMatcher intLiteralToCharMatcherPL;
    //match int x = 1; char c = x;
    extern DeclarationMatcher intVarToCharMatcherPL;
    //match i = c; or c = i
    extern StatementMatcher equOpMatcherPL;

    class CharIntPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
