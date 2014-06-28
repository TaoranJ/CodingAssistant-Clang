#ifndef _MATCH_VOID_POINTER_H
#define _MATCH_VOID_POINTER_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //match void*
    extern DeclarationMatcher voidPointerMatcherPL;
    //match void**
    extern DeclarationMatcher voidPtrPtrMatcherPL;
    //match void* []
    extern DeclarationMatcher voidPtrArrayMatcherPL;

    class VoidPointerPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
