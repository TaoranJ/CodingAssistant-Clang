#ifndef _MATCH_STRUCT_H
#define _MATCH_STRUCT_H

#include "ASTUtility.h" 

namespace ProgrammingLanguage {

    using namespace clang::ast_matchers;

    //match sturct/union/class 
    extern DeclarationMatcher recordDeclMatcherPL;

    bool TypeSize(const std::vector<std::string>& typeList);
    extern int countStruct;

    class RecordDeclPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult &Result);
    };
}
#endif
