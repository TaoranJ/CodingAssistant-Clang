#include "ASTUtility.h" 
#include <fstream>
#include <map>
#include <vector>
#include <string>

//match a function definition, and this function at least has 1 parameter which
//is a reference type.
//StatementMatcher bodyMatcher = compoundStmt(hasParent(
//           functionDecl(hasAnyParameter(hasType(referenceType()))).bind("func"))
//           ).bind("body");
DeclarationMatcher funcMatcher = functionDecl(
        hasAnyParameter(hasType(referenceType())),
        has(compoundStmt().bind("body"))).bind("func");

std::map< std::string, std::vector<std::string> >funcParm; //<funcName, non-const-ref parm vector>
std::map< std::string, std::vector<std::string> >funcParmUsed;

void LoopThroughCS(const clang::Stmt* funcBody, const clang::ASTContext* Context, const std::string func)
{
    //
    //leaf
    if (funcBody -> children().empty())
    {
        if (clang::DeclRefExpr::classof(funcBody))
        {
            const clang::Decl* d = static_cast<const clang::DeclRefExpr*>(funcBody) -> getDecl();
            if (clang::ParmVarDecl::classof(d) && 
                    static_cast<const clang::ValueDecl*>(d) -> getType() -> isReferenceType())
            {
                funcParmUsed[func].push_back(d -> getLocation().printToString(Context -> getSourceManager()));
            }
        }
        return;
    }

    for(auto s : funcBody -> children())
    {
        if (!s) continue;
        //for UnaryOperator
        if (clang::UnaryOperator::classof(s))
        {
            const clang::UnaryOperator* op = static_cast<const clang::UnaryOperator*>(s);
            if (op -> isIncrementDecrementOp())
            {
                LoopThroughCS(op -> getSubExpr(), Context, func);
            }
            else
            {
                LoopThroughCS(s, Context, func);
            }
        }
        //for BinaryOperator
        else if (clang::BinaryOperator::classof(s))
        {
            const clang::BinaryOperator* op = static_cast<const clang::BinaryOperator*>(s);
            if (op -> isCompoundAssignmentOp() || op -> isAssignmentOp())
            {
                LoopThroughCS(op -> getLHS(), Context, func);
            }
            else
            {
                LoopThroughCS(s, Context, func);
            }
        }
        //for CallExpr
        else if (clang::CallExpr::classof(s))
        {
            const clang::CallExpr* call = static_cast<const clang::CallExpr*>(s);
            
            for (auto i = const_cast<clang::CallExpr*>(call) -> arg_begin(); i != const_cast<clang::CallExpr*>(call) -> arg_end() ; ++i)
            {
                //ref vs ref
                if (clang::DeclRefExpr::classof(*i))
                {
                    clang::Decl* d = static_cast<clang::DeclRefExpr*>(*i) -> getDecl();
                    if (clang::ParmVarDecl::classof(d) && 
                            static_cast<clang::ValueDecl*>(d) -> getType() -> isReferenceType())
                    {
                        funcParmUsed[func].push_back(d -> getLocation().printToString(Context -> getSourceManager()));
                    }
                }
                //call const int&, i have only int &
                else if (clang::ImplicitCastExpr::classof(*i))
                {
                    if (static_cast<clang::ImplicitCastExpr*>(*i) -> getCastKind() == clang::CastKind::CK_NoOp)
                    {
                        for(auto j : (*i) -> children())
                        {
                            if (clang::DeclRefExpr::classof(j))
                            {
                                clang::Decl* d = static_cast<clang::DeclRefExpr*>(j) -> getDecl();
                                if (clang::ParmVarDecl::classof(d) &&
                                    static_cast<clang::ValueDecl*>(d) -> getType() -> isReferenceType())
                                {
                                    funcParmUsed[func].push_back(d -> getLocation().printToString(Context -> getSourceManager()));
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            LoopThroughCS(s, Context, func);
        }
    }
}
class FuncPrinter : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) 
    {
        //get the node
        clang::ASTContext *Context = Result.Context;
        const clang::FunctionDecl *func = Result.Nodes.getNodeAs<clang::FunctionDecl>("func");
        const clang::CompoundStmt *funcBody = Result.Nodes.getNodeAs<clang::CompoundStmt>("body");
        
        if (!funcBody || ASTUtility::IsStmtInSTDFile(funcBody, Context)) return;
        if (!func || ASTUtility::IsDeclInSTDFile(func, Context)) return;

        auto funcName = func -> getLocation().printToString(Context -> getSourceManager());
        //function body
        LoopThroughCS(funcBody, Context, funcName);

        for(auto parm : func -> parameters())
        {
            if (!parm -> getType() -> isReferenceType()) continue;
            if (parm -> getType().getNonReferenceType().isConstQualified()) return;
            funcParm[funcName].push_back(parm -> getLocation().printToString(Context -> getSourceManager()));
        }
    }
};

//"My tool options" is the name.
static llvm::cl::OptionCategory MyToolCategory("My tool options");

/******************************************************************************
 *
 *****************************************************************************/
int main(int argc, const char **argv)
{
    //CommonOptionsParser constructor will parse arguments and create a
    //CompilationDatabase. In case of error it will terminate the program.
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    //once we have a CompilationDatabase, we can create a ClangTool and run our
    //FrontendAction over some code.
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    //tooling::MyFactory Factory;
    FuncPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(funcMatcher, &Printer);

    Tool.run(newFrontendActionFactory(&Finder));

    for (auto p1 : funcParm)
    {
        for(auto p2 : p1.second) 
        {
            bool ret = false;
            for(auto p3 : funcParmUsed.at(p1.first))
            {
                if (p2 == p3)
                {
                    ret = true;
                    break;
                }
            }
            if(!ret) 
            {
                std::ofstream out("Rule037", std::ofstream::app);
                out << p2 << "\n";
                out.close();
            }
        }
    }
    return 0;
}
