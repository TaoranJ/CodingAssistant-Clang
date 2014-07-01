#include "../include/MatchStruct.h" 

namespace ProgrammingLanguage {
    
    //match sturct/union/class 
    DeclarationMatcher recordDeclMatcherPL = recordDecl(has(fieldDecl())).bind("recordDecl");
    int count = 0;
}

bool ProgrammingLanguage::TypeSize(const std::vector<std::string>& typeList)
{
    std::vector<int> typeSize;

    for (auto i : typeList)
    {
        if(i == "char" || i == "signed char" || i == "unsigned char")
            typeSize.push_back(1);
        else if (i == "short" || i == "unsigned short" || i == "char16_t")
            typeSize.push_back(2);
        else if(i == "int" || i == "unsigned int" || i == "float" || i == "char32_t" || i == "pointer")
            typeSize.push_back(4);
        else if(i == "long" || i == "unsigned long")
            typeSize.push_back(5);
        else if(i == "long long" || i == "unsigned long long" || i == "double")
            typeSize.push_back(8);
        else if(i == "long double")
            typeSize.push_back(12);
        else if(i == "__int128" || i == "unsigned __int128")
            typeSize.push_back(16);
        else 
            typeSize.push_back(0);
    }

    for(auto i : typeSize)
        for(auto j : typeSize)
            if(i < j && i != 0) 
                return false;
    return true;    
}

void ProgrammingLanguage::RecordDeclPrinter::run(const MatchFinder::MatchResult &Result) 
{
    //get the node
    clang::ASTContext *Context = Result.Context;
    const clang::RecordDecl *st = Result.Nodes.getNodeAs<clang::RecordDecl>("recordDecl");
    std::vector<std::string> typeList;

    //only need struct
    if(!st || !st -> isCompleteDefinition() || !st -> isStruct() || ASTUtility::IsDeclInSTDFile(st, Context)) return;

    count++;
    std::ofstream out(stdout);
    if (!out) return; 
    out << st->getNameAsString() << "\n";

    for(clang::RecordDecl::field_iterator it = st -> field_begin(); it != st -> field_end(); it++)
    {
        if (!(*it)) continue;

        if((*it) -> getType() -> isBuiltinType())
			out << (*it)->getType().getAsString();
        else if((*it) -> getType() -> isPointerType())
			out << "pointer\n";
        else
			out << "nothing\n";
    }

    if (!TypeSize(typeList))
        ASTUtility::Print(st, Context, "Rule023");
}
