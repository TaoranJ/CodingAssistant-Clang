#ifndef _MATCH_LAMBDA_H
#define _MATCH_LAMBDA_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //match lambda expr 
    extern StatementMatcher lambdaMatcherPL;
    void RemoveWhiteSpace(std::string& str);
    bool IsLambdaTooLong(const std::string str);
    bool IsThereADeduction(const std::string& header, const std::size_t& pos);
    class LambdaPrinter : public MatchFinder::MatchCallback { 
        public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
