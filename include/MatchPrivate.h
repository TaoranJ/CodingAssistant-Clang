#ifndef _MATCH_PRIVATE_H
#define _MATCH_PRIVATE_H

#include "ASTUtility.h"

namespace EffectiveCPP {
    using namespace clang::ast_matchers;
    extern DeclarationMatcher recordDeclMatcherEC;

    class RecordPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
