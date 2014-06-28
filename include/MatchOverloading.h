#ifndef _MATCH_OVERLOADING_H
#define _MATCH_OVERLOADING_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    extern StatementMatcher allFuncMatcherPL;
    extern std::map<std::string, int> countOverloding;

    class OverloadingFuncPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result); 
    };
}
#endif

