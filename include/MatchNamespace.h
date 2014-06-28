#ifndef _MATCH_NAMESAPCE_H
#define _MATCH_NAMESAPCE_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //match namespace 
    extern DeclarationMatcher namespaceMatcherPL;

    class NamespacePrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result); 
    };
}
#endif
