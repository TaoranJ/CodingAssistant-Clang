#ifndef _MATCH_STD_H
#define _MATCH_STD_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //match function here
    extern StatementMatcher funcCallMatcherPL;
    //match var declarations here
    extern DeclarationMatcher varMatcherPL;

    class STDPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
