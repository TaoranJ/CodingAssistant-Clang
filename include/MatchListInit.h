#ifndef _MATCH_LIST_INIT_H
#define _MATCH_LIST_INIT_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;   

    //int x = {2}     or int x{2}
    extern DeclarationMatcher varDeclMatcherPL;
    //for class
    extern DeclarationMatcher classVarDeclMatcherPL;

    class InitListPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run (const MatchFinder::MatchResult& Result);
    };
}
#endif
