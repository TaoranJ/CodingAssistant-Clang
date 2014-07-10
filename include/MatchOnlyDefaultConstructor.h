#ifndef _MATCH_ONLY_DEFAULT_CONSTRUCTOR_H
#define _MATCH_ONLY_DEFAULT_CONSTRUCTOR_H 

#include "ASTUtility.h"
//Implementation of chater 16 advice 4th
namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //mathch class with only default constructor;
    extern DeclarationMatcher classOnlyDefaultConstructorMatcherPL;
    
    class OnlyDefaultConstructorClassPrinter : public MatchFinder::MatchCallback {
        public:
            virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
