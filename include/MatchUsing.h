#ifndef _MATCH_USING_H
#define _MATCH_USING_H

#include "ASTUtility.h"

namespace ProgrammingLanguage {
    using namespace clang::ast_matchers;

    //only using std:vector
    //no using namespace std;
    extern DeclarationMatcher usingDeclMatcherPL;
    
    bool CheckHeader(std::string fileName);
    class UsingPrinter : public MatchFinder::MatchCallback {
    public:
    	virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
