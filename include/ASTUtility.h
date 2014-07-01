#ifndef _ASTUTILITY_H
#define _ASTUTILITY_H

#include <string> 
#include <fstream>
#include <iostream>
#include <cmath>

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
        static double CalculateSimilarity(std::string sourceStr, std::string targetStr)
        {
            int val = LD(sourceStr, targetStr);
            return 1.0 - (double)val / fmax(sourceStr.length(), targetStr.length());
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
        static int threeMin(int a, int b, int c)
        {
            return fmin(fmin(a, b), c);
        }
        static int LD(std::string left, std::string right)
        {
            if (0 == left.length() || 0 == right.length())
                return fmax(left.length(), right.length());
 
            
            //init
            std::vector< std::vector<int> >LD;
            std::vector<int> row;
            for (int j = 0; j <= right.length(); j++)
            {
                for (int i = 0; i <= left.length(); i++)
                    row.push_back(0);
                LD.push_back(row);
                row.clear();
            }
            for (int i = 0; i <= left.length(); i++) 
                LD[i][0] = i;
            for (int j = 0; j <= right.length(); j++) 
                LD[0][j] = j;
                
            //calculate
            for (int i = 1; i <= left.length(); i++)
                for (int j = 1; j <= right.length(); j++)
                    LD[i][j] = threeMin(LD[i - 1][j] + 1, LD[i][j - 1] + 1, 
                            LD[i - 1][j - 1] + left[i -1] == right[j - 1]? 0 : 1);
            return LD[left.length()][right.length()];
        }
};
using namespace clang::ast_matchers;

#endif
