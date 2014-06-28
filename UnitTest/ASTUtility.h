#include <string> 
#include <fstream>
#include <iostream>

#include "llvm/Support/CommandLine.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Parse/Parser.h"
#include "clang/AST/PrettyPrinter.h" 
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

//ast_match
#include "clang/ASTMatchers/ASTMatchers.h" 
#include "clang/ASTMatchers/ASTMatchFinder.h" 

using namespace clang::driver;
using namespace clang::tooling;
using namespace clang::ast_matchers;


class ASTUtility { 
    public: 
        //format output for Decl cout
        static std::string Print(const clang::Decl* d, const clang::ASTContext* Context)
        {
            auto loc = GetVarLoc(d, Context);                    
            if (!loc.compare("<invalid loc>")) return "\n";
            auto name = GetName(d, Context);            
            return loc + ":" + name;
        }

        //format output for Decl file
        static void Print(const clang::Decl* d, const clang::ASTContext* Context, const std::string& filename)
        {
            std::ofstream out(filename, std::ostream::app);  
            if(!out) return;

            auto loc = GetVarLoc(d, Context);                    
            if (!loc.compare("<invalid loc>"))
            {
                out <<  "\n";
                return;
            }
            auto name = GetName(d, Context);            
            out << loc + ":" + name + "\n";
        }

        //fromat output for Stmt cout
        static std::string Print(const clang::Stmt* s, const clang::ASTContext* Context)
        {
            auto loc = GetStmtLoc(s, Context);                    
            if (!loc.compare("<invalid loc>")) return "\n";
            auto name = GetName(s, Context);            
            return loc + ":" + name;
        }
        
        //fromat output for Stmt file
        static void Print(const clang::Stmt* s, const clang::ASTContext* Context, const std::string& filename)
        {
            std::ofstream out(filename, std::ostream::app);  
            if(!out) return;

            auto loc = GetStmtLoc(s, Context);                    
            if (!loc.compare("<invalid loc>"))
            {
                out << "\n";
                return;
            }
            auto name = GetName(s, Context);            
            out << loc + ":" + name + "\n";
        }

        //in std file
        static bool IsDeclInSTDFile(const clang::Decl* d, const clang::ASTContext* Context)
        {
            auto loc = GetVarLoc(d, Context);                    
            if (!loc.compare("<invalid loc>")) return true;
            return IsInSTDFile(loc);
        }
        static bool IsStmtInSTDFile(const clang::Stmt* s, const clang::ASTContext* Context)
        {
            auto loc = GetStmtLoc(s, Context);                    
            if (!loc.compare("<invalid loc>")) return true;
            return IsInSTDFile(loc);
        }

        //std container or classed inherites from std container
        static bool IsSTDContainer(const clang::Decl* d)
        {
            //if it is a CXXRecordDecl
            if ( !clang::CXXRecordDecl::classof(d) ) return false; 

            auto CRD = static_cast<const clang::CXXRecordDecl*>(d);

            //is a std container
            if (IsContainer(CRD -> getQualifiedNameAsString())) return true;   

            //inherits from a container
            for (auto i = CRD -> bases_begin(); i != CRD -> bases_end(); i++)
                if(IsContainer(i -> getTypeSourceInfo() -> getType().getAsString()))
                        return true;
            return false;
        }

        static bool IsInTheFile(const clang::Decl* d, const clang::ASTContext* Context)
        {
            return Context -> getSourceManager().isInMainFile(d -> getLocStart());
        }
        static bool IsInTheFile(const clang::Stmt* s, const clang::ASTContext* Context)
        {
            return Context -> getSourceManager().isInMainFile(s -> getLocStart());
        }
    private:
        static std::string GetName(const clang::Stmt* s, const clang::ASTContext* Context)
        {
            std::string name;
            llvm::raw_string_ostream rss(name);
            s -> printPretty(rss, nullptr, clang::PrintingPolicy(Context -> getLangOpts()));
            return rss.str();
        }
        static std::string GetName(const clang::Decl* d, const clang::ASTContext* Context)
        {
            std::string name;
            llvm::raw_string_ostream rss(name);
            d -> print(rss, clang::PrintingPolicy(Context -> getLangOpts()));
            return rss.str();
        }

        //get the start location of a statement
        static std::string GetStmtLoc(const clang::Stmt* s, const clang::ASTContext* Context) 
        {
            return s -> getLocStart().printToString(Context -> getSourceManager());
        }
        //get the location of a variable declaration
        static std::string GetVarLoc(const clang::Decl* d, const clang::ASTContext* Context)
        {
            return d -> getLocStart().printToString(Context -> getSourceManager());
        }

        static bool IsInSTDFile(const std::string& location)
        {
            return (location.find("/usr/include") != std::string::npos || location.find("/usr/local") != std::string::npos || location.find("/usr/lib") != std::string::npos) ? true : false;
        }
        static bool IsContainer(const std::string& name)
        {
            if(name.find("std::array") != std::string::npos || name.find("std::vector") != std::string::npos ||
                    name.find("std::deque") != std::string::npos ||
                    name.find("std::forward_list") != std::string::npos ||
                    name.find("std::list") != std::string::npos ||
                    name.find("std::stack") != std::string::npos ||
                    name.find("std::queue") != std::string::npos ||
                    name.find("std::priority_queue") != std::string::npos || 
                    name.find("std::set") != std::string::npos ||
                    name.find("std::multiset") != std::string::npos ||
                    name.find("std::map") != std::string::npos ||
                    name.find("std::multimap") != std::string::npos ||
                    name.find("std::unordered_set") != std::string::npos ||
                    name.find("std::unordered_multiset") != std::string::npos ||
                    name.find("std::unordered_map") != std::string::npos ||
                    name.find("std::unordered_multimap") != std::string::npos)
                return true;
            return false;
        }
};
