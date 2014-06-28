#ifndef _MATCH_SIMILAR_NAME_H
#define _MATCH_SIMILAR_NAME_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    extern const int namelengthThreshold;
    extern int similarPairCount;
    extern std::vector<std::string> nameVec;
    extern DeclarationMatcher varNameMatcherPL;

    class SimilarNamePrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
