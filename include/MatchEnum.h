#ifndef _MATCH_ENUM_H
#define _MATCH_ENUM_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //match sturct/union/class 
    extern DeclarationMatcher enumDeclMatcherPL;

    class EnumDeclPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
