#ifndef _MATCH_CONTAINER_H
#define _MATCH_CONTAINER_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;
    //match overloaded []
    extern StatementMatcher callMatcherPL;

    class CallPrinter : public MatchFinder::MatchCallback {
        public:
            virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
