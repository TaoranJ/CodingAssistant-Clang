#ifndef _MATCH_MAGIC_NUMBER_H
#define _MATCH_MAGIC_NUMBER_H

#include "ExceptNumber.h"
#include "ASTUtility.h"
 
namespace CodStd101 {
    using namespace clang::ast_matchers;

    extern StatementMatcher floatLiteralMatcherCS;
    extern StatementMatcher intLiteralMatcherCS;
    extern StatementMatcher stringLiteralMatcherCS;

    class FloatPrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
    class IntPrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
    class StringPrinter : public MatchFinder::MatchCallback {
    public:	
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
