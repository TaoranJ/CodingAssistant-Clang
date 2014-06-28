#ifndef _MATCH_HANDLE_CLASS_H
#define _MATCH_HANDLE_CLASS_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //match the the expr who calls a class constructor
    extern StatementMatcher classConstructMatcherPL;

    class NewExprDeclPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
