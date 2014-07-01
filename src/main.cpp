#include "../include/ProgrammingLanguage.h"
#include "../include/CodStd101.h"
#include "../include/EffectiveCPP.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Basic/TargetInfo.h"
#include "llvm/Support/Host.h"
#include "clang/Parse/ParseAST.h"

//"My tool options" is the name.
static llvm::cl::OptionCategory MyToolCategory("My tool options");

class PrintFunctionsASTConsumer : public clang::ASTConsumer
{
public:
	virtual bool HandleTopLevelDecl(clang::DeclGroupRef DG)
	{
		for (auto pDecl : DG)
		{
			if (const clang::NamedDecl *pND = llvm::dyn_cast<clang::NamedDecl>(pDecl))
			{
				llvm::errs() << "top-level-decl: " << pND->getNameAsString()
					<< "\n\tKind: [" << pND->getDeclKindName() << "] "
					<< "\n\tQualName: " << pND->getQualifiedNameAsString()
					<< "\n";
				clang::Decl::Kind declKind = pND->getKind();

				const clang::FunctionDecl* pfnDecl = llvm::dyn_cast<clang::FunctionDecl>(pDecl);
				if (pfnDecl == nullptr || !pfnDecl->isFirstDecl()) continue;

				llvm::errs() << pfnDecl->getReturnType().getAsString()
					<< " " << pfnDecl->getName()
					<< "(";
				for (auto param : pfnDecl->params())
				{
					llvm::errs() << param->getType().getAsString() << ", ";
				}
				llvm::errs() << ");";
			}
		}
		return true;
	}
};


class ClangParser
{
	clang::CompilerInstance m_CompilerInstance;

	void prepareCompilerforFile(const char* szSourceCodeFilePath)
	{
		// To reuse the source manager we have to create these tables.
		m_CompilerInstance.getSourceManager().clearIDTables();
		//m_CompilerInstance.InitializeSourceManager(clang::FrontendInputFile(szSourceCodeFilePath, clang::InputKind::IK_C));// ger(m_CompilerInstance.getFileManager());

		// supply the file to the source manager and set it as main-file 
		const clang::FileEntry * file = m_CompilerInstance.getFileManager().getFile(szSourceCodeFilePath);
		clang::FileID fileID = m_CompilerInstance.getSourceManager().createFileID(file, clang::SourceLocation(), clang::SrcMgr::CharacteristicKind::C_User);
		m_CompilerInstance.getSourceManager().setMainFileID(fileID);

		// CodeGenAction needs this
		clang::FrontendOptions& feOptions = m_CompilerInstance.getFrontendOpts();
		feOptions.Inputs.clear();
		feOptions.Inputs.push_back(clang::FrontendInputFile(szSourceCodeFilePath, clang::FrontendOptions::getInputKindForExtension(clang::StringRef(szSourceCodeFilePath).rsplit('.').second), false));
	}
public:
	ClangParser()
	{
		// Usually all examples try to build the CompilerInstance Object from CompilerInvocation object.
		// However, CompilterInstance already has an in-built CompilerInvocation object. So we use it.
		// Only problem is: target options are not set correctly for the in-built object. We do it manually.
		// This below line is just to assert that the object exist.
		clang::CompilerInvocation& invocation = m_CompilerInstance.getInvocation();

		// Diagnostics is needed - set it up
		m_CompilerInstance.createDiagnostics();

		// set the include directories path
		clang::HeaderSearchOptions & headerSearchOptions = m_CompilerInstance.getHeaderSearchOpts();
		headerSearchOptions.AddPath("C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\include", clang::frontend::IncludeDirGroup::Angled, false, false);
		// headerSearchOptions.Verbose = true;

		// set few options
		clang::LangOptions& langOptions = m_CompilerInstance.getLangOpts();
		langOptions.CPlusPlus = 1;
		langOptions.Bool = 1;
		langOptions.RTTI = 0;
#if defined(_MSC_VER)
		langOptions.MicrosoftExt = 1;
		langOptions.MSVCCompat = 1;
		langOptions.MSBitfields = 1;
		langOptions.DelayedTemplateParsing = 1; // MSVC parses templates at the time of actual use
		m_CompilerInstance.getDiagnosticOpts().setFormat(clang::TextDiagnosticFormat::Msvc);
		//clInstance.getTargetOpts().ABI = "microsoft";
#endif

		// Need to set the source manager before AST
		m_CompilerInstance.createFileManager();
		m_CompilerInstance.createSourceManager(m_CompilerInstance.getFileManager());

		// Need to set the target before AST. Adjust the default target options and create a target
		m_CompilerInstance.getTargetOpts().Triple = llvm::sys::getProcessTriple();
		m_CompilerInstance.setTarget(clang::TargetInfo::CreateTargetInfo(m_CompilerInstance.getDiagnostics(), &m_CompilerInstance.getTargetOpts()));

		// Create pre-processor and AST Context
		m_CompilerInstance.createPreprocessor(clang::TranslationUnitKind::TU_Module);
		m_CompilerInstance.createASTContext();
		if (m_CompilerInstance.hasPreprocessor())
		{
			clang::Preprocessor & preprocessor = m_CompilerInstance.getPreprocessor();
			preprocessor.getBuiltinInfo().InitializeBuiltins(preprocessor.getIdentifierTable(), preprocessor.getLangOpts());
		}
	}

	bool parseAST(const char* szSourceCodeFilePath, clang::ast_matchers::MatchFinder finder)
	{
		// create the compiler instance setup for this file as main file
		prepareCompilerforFile(szSourceCodeFilePath);

		std::unique_ptr<clang::ASTConsumer> pAstConsumer (finder.newASTConsumer());
		
		clang::DiagnosticConsumer& diagConsumer = m_CompilerInstance.getDiagnosticClient();
		diagConsumer.BeginSourceFile(m_CompilerInstance.getLangOpts(), &m_CompilerInstance.getPreprocessor());
		clang::ParseAST(m_CompilerInstance.getPreprocessor(), pAstConsumer.get(), m_CompilerInstance.getASTContext());
		diagConsumer.EndSourceFile();

		return diagConsumer.getNumErrors() != 0;
	}

	// Executes CodeGenAction and returns the pointer (caller should own and delete it)
	// Returns NULL on failure (in case of any compiler errors etc.)
	clang::CodeGenAction* emitLLVM(const char* szSourceCodeFilePath)
	{
		// create the compiler instance setup for this file as the main file
		prepareCompilerforFile(szSourceCodeFilePath);

		// Create an action and make the compiler instance carry it out
		clang::CodeGenAction *Act = new clang::EmitLLVMOnlyAction();
		if (!m_CompilerInstance.ExecuteAction(*Act))
			return NULL;

		return Act;
	}

	clang::ASTContext& GetASTContext() { return m_CompilerInstance.getASTContext(); }
};


int main(int argc, const char **argv)
{
    clang::ast_matchers::MatchFinder Finder;

    //Rule001 MatchCString
    ProgrammingLanguage::CharPointerPrinter Printer001;
    Finder.addMatcher(ProgrammingLanguage::charPointerMatcherPL, &Printer001);
    Finder.addMatcher(ProgrammingLanguage::charArrayMatcherPL, &Printer001);
    Finder.addMatcher(ProgrammingLanguage::arrayOfCharPointerMatcherPL, &Printer001);
    Finder.addMatcher(ProgrammingLanguage::charPtrPtrMatcherPL, &Printer001);

    //Rule002 && Rule003 MatchArray
    ProgrammingLanguage::ArrayPrinter Printer002;
    Finder.addMatcher(ProgrammingLanguage::arrayMatcherPL, &Printer002);

    //Rule004 && Rule005 MatchContainer
    ProgrammingLanguage::CallPrinter Printer003;
    Finder.addMatcher(ProgrammingLanguage::callMatcherPL, &Printer003);

    //Rule006 MatchMainCatch
    ProgrammingLanguage::TryInFuncPrinter Printer004;
    Finder.addMatcher(ProgrammingLanguage::tryInFuncMatcherPL, &Printer004);

    //Rule007 MatchRealloc
    ProgrammingLanguage::CallExprPrinter Printer005;
    Finder.addMatcher(ProgrammingLanguage::callExprMatcherPL, &Printer005);

    //Rule008 MatchNotPlainChar
    ProgrammingLanguage::NotPlainCharPrinter Printer006;
    Finder.addMatcher(ProgrammingLanguage::notPlainCharMatcherPL, &Printer006);

    //Rule009 MatchSignedCast
    ProgrammingLanguage::ImpCastExprPrinter Printer007;
    Finder.addMatcher(ProgrammingLanguage::impCastExprMatcherPL, &Printer007);

    //Rule010 MatchMultiDeclarations
    ProgrammingLanguage::MultiDeclPrinter Printer008;
    Finder.addMatcher(ProgrammingLanguage::declStmtMatcherPL, &Printer008);

    //Rule011 MatchUninitVar
    ProgrammingLanguage::UninitVarPrinter Printer009;
    Finder.addMatcher(ProgrammingLanguage::uninitializedVarMatcherPL, &Printer009);

    //Rule012 MatchAuto
    ProgrammingLanguage::AutoVarPrinter Printer010;
    Finder.addMatcher(ProgrammingLanguage::autoVarMatcherPL, &Printer010);

    //Rule013 && Rule014 MatchInitList
    ProgrammingLanguage::InitListPrinter Printer011;
    Finder.addMatcher(ProgrammingLanguage::varDeclMatcherPL, &Printer011);
    Finder.addMatcher(ProgrammingLanguage::classVarDeclMatcherPL, &Printer011);

    //Rule015 MatchIntegerToChar
    ProgrammingLanguage::CharIntPrinter Printer012;
    Finder.addMatcher(ProgrammingLanguage::charLiteralToIntMatcherPL, &Printer012);
    Finder.addMatcher(ProgrammingLanguage::intLiteralToCharMatcherPL, &Printer012);
    Finder.addMatcher(ProgrammingLanguage::intVarToCharMatcherPL, &Printer012);
    Finder.addMatcher(ProgrammingLanguage::equOpMatcherPL, &Printer012);

    //Rule016 MatchMultidimArrays
    ProgrammingLanguage::MultiArrayPrinter Printer013;
    Finder.addMatcher(ProgrammingLanguage::multiArrayMatcherPL, &Printer013);
    
    //Rule017 MatchNull
    ProgrammingLanguage::NullPointerPrinter Printer014;
    Finder.addMatcher(ProgrammingLanguage::nullPointerMatcherPL, &Printer014);
    Finder.addMatcher(ProgrammingLanguage::biOpMatcher1PL, &Printer014);
    Finder.addMatcher(ProgrammingLanguage::biOpMatcher2PL, &Printer014);
    Finder.addMatcher(ProgrammingLanguage::biOpMatcher3PL, &Printer014);

    //Rule018 MatchRefParm
    ProgrammingLanguage::ParmPrinter Printer015;
    Finder.addMatcher(ProgrammingLanguage::funcMatcherPL, &Printer015);
    
    //Rule019 && Rule020 MatchHandleClass
    ProgrammingLanguage::NewExprDeclPrinter Printer016;
    Finder.addMatcher(ProgrammingLanguage::classConstructMatcherPL, &Printer016);
    
    //Rule021 MatchNakedNew
    ProgrammingLanguage::NakedNewPrinter Printer017;
    Finder.addMatcher(ProgrammingLanguage::nakedNewMatcherPL, &Printer017);

    //Rule022 MatchVoidPointer
    ProgrammingLanguage::VoidPointerPrinter Printer018;
    Finder.addMatcher(ProgrammingLanguage::voidPointerMatcherPL, &Printer018);
    Finder.addMatcher(ProgrammingLanguage::voidPtrPtrMatcherPL, &Printer018);
    Finder.addMatcher(ProgrammingLanguage::voidPtrArrayMatcherPL, &Printer018);

    //Rule023 MatchStruct
    ProgrammingLanguage::RecordDeclPrinter Printer019;
    Finder.addMatcher(ProgrammingLanguage::recordDeclMatcherPL, &Printer019);

    //Rule024 MatchBitField
    ProgrammingLanguage::BitFieldPrinter Printer020;
    Finder.addMatcher(ProgrammingLanguage::recordFieldDeclMatcherPL, &Printer020);

    //Rule025 MatchEnum
    ProgrammingLanguage::EnumDeclPrinter Printer021;
    Finder.addMatcher(ProgrammingLanguage::enumDeclMatcherPL, &Printer021);

    //Rule026 MatchForStmt
    ProgrammingLanguage::ForRangeStmtPrinter Printer022;
    Finder.addMatcher(ProgrammingLanguage::forStmtMatcherPL, &Printer022);
    Finder.addMatcher(ProgrammingLanguage::forRangeStmtMatcherPL, &Printer022);

    //Rule027 MatchDoStmt
    ProgrammingLanguage::DoStmtPrinter Printer023;
    Finder.addMatcher(ProgrammingLanguage::doStmtMatcherPL, &Printer023);

    //Rule028 MatchGotoStmt
    ProgrammingLanguage::GotoStmtPrinter Printer024;
    Finder.addMatcher(ProgrammingLanguage::gotoStmtMatcherPL, &Printer024);

    //Rule029 MatchSTD
    ProgrammingLanguage::STDPrinter Printer025;
    Finder.addMatcher(ProgrammingLanguage::funcCallMatcherPL, &Printer025);
    Finder.addMatcher(ProgrammingLanguage::varMatcherPL, &Printer025);

    //Rule030 MatchNarrowingConversion
    ProgrammingLanguage::NarrowCastPrinter Printer026;
    Finder.addMatcher(ProgrammingLanguage::norCastMatcherPL, &Printer026);

    //Rule031 MatchNewExpr
    ProgrammingLanguage::NewExprPrinter Printer027;
    Finder.addMatcher(ProgrammingLanguage::cxxNewExprMatcherPL, &Printer027);

    //Rule032 && Rule033 MatchLambda
    ProgrammingLanguage::LambdaPrinter Printer028;
    Finder.addMatcher(ProgrammingLanguage::lambdaMatcherPL, &Printer028);

    //Rule034 MatchFunCast
    ProgrammingLanguage::FunctionalCastPrinter Printer029;
    Finder.addMatcher(ProgrammingLanguage::funcCastMatcherPL, &Printer029);

    //Rule035 MatchDynamicCast
    ProgrammingLanguage::DynamicCastPrinter Printer030;
    Finder.addMatcher(ProgrammingLanguage::dynamicCastMatcherPL, &Printer030);

    //Rule036 MatchFuncConstexpr
    ProgrammingLanguage::FunctionPrinter Printer031;
    Finder.addMatcher(ProgrammingLanguage::normalFuncMatcherPL, &Printer031);

    //Rule037 MatchShouldBeConstRef
    ProgrammingLanguage::FuncRefParmPrinter Printer032;
    Finder.addMatcher(ProgrammingLanguage::funcRefParmMatcherPL, &Printer032);
    for (auto p1 : ProgrammingLanguage::funcParm)
    {
        for(auto p2 : p1.second) 
        {
            bool ret = false;
            for(auto p3 : ProgrammingLanguage::funcParmUsed.at(p1.first))
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

    //Rule038 MatchPassByValue
    ProgrammingLanguage::FuncParmPrinter Printer033;
    Finder.addMatcher(ProgrammingLanguage::funcParmMatcherPL, &Printer033);

    //Rule039 MatchReturnWarning
    ProgrammingLanguage::ReturnPrinter Printer034;
    Finder.addMatcher(ProgrammingLanguage::returnMatcherPL, &Printer034);

    //Rule040 MatchAryAsPtr
    ProgrammingLanguage::ArrayAsPtrPrinter Printer035;
    Finder.addMatcher(ProgrammingLanguage::simpleImpCastMatcherPL, &Printer035);

    //Rule041 MatchVarParm
    ProgrammingLanguage::FunctionVarParmPrinter Printer036;
    Finder.addMatcher(ProgrammingLanguage::funcVarParmMatcherPL, &Printer036);

    //Rule042 MatchOverloading
    ProgrammingLanguage::OverloadingFuncPrinter Printer037;
    Finder.addMatcher(ProgrammingLanguage::allFuncMatcherPL, &Printer037);
    for(auto it : ProgrammingLanguage::countOverloding)
    {
        if (it.second >= 2 && it.second <= 4)
        {
            std::ofstream out("Rule042", std::ofstream::app); 
            out << it.first << " : " << it.second << '\n';
            out.close();
        }
    }

    //Rule043 MatchNamespace
    ProgrammingLanguage::NamespacePrinter Printer038;
    Finder.addMatcher(ProgrammingLanguage::namespaceMatcherPL, &Printer038);

    //Rule045 && Rule046 && Rule047 && Rule048  MatchCast
    ProgrammingLanguage::ExplicitCastPrinter Printer039;
    Finder.addMatcher(ProgrammingLanguage::explicitCastMatcherPL, &Printer039);

    //Rule049 MatchMagicNumber
    CodStd101::IntPrinter Printer040;
    CodStd101::FloatPrinter Printer041;
    CodStd101::StringPrinter Printer042;
    Finder.addMatcher(CodStd101::intLiteralMatcherCS, &Printer040);
    Finder.addMatcher(CodStd101::floatLiteralMatcherCS, &Printer041);
    Finder.addMatcher(CodStd101::stringLiteralMatcherCS, &Printer042);

    //Rule050 MatchFuncLines
    CodStd101::FuncLinesPrinter Printer043;
    Finder.addMatcher(CodStd101::funcStmtMatcherCS, &Printer043);

    //Rule051 && Rule052 MatchGlobalVar
    CodStd101::GlobalVarPrinter Printer044;
    Finder.addMatcher(CodStd101::normalVarDeclMatcherCS, &Printer044);

    //Rule053 MatchSimilarName
    ProgrammingLanguage::SimilarNamePrinter Printer045;
    Finder.addMatcher(ProgrammingLanguage::varNameMatcherPL, &Printer045);
	for(int i = 0; i < ProgrammingLanguage::nameVec.size(); ++i)
    {
		std::string oneName(ProgrammingLanguage::nameVec[i]);
		for(int j = i + 1; j < ProgrammingLanguage::nameVec.size(); ++j)
        {
			std::string anotherName(ProgrammingLanguage::nameVec[j]);
			if(ASTUtility::CalculateSimilarity(oneName,anotherName) >= 0.5 && ASTUtility::CalculateSimilarity(oneName,anotherName) != 1)
            {
                std::ofstream out("Rule053", std::ostream::app);
                out << oneName << " : " << anotherName << "\n";
                out.close();
			}
		}
	}

    //Rule054 MatchUninline
    ProgrammingLanguage::UnInlineFuncPrinter Printer046;
    Finder.addMatcher(ProgrammingLanguage::functionDeclMatcherPL, &Printer046);

    //Rule055 MatchVirtualCallee
    EffectiveCPP::CtorCallVtlPrinter Printer047;
    Finder.addMatcher(EffectiveCPP::ctorCallVtlMatcherEC, &Printer047);
    Finder.addMatcher(EffectiveCPP::dtorCallVtlMatcherEC, &Printer047);

    //Rule056 MatchVarDefine
    EffectiveCPP::VarDefinePrinter Printer048;
    Finder.addMatcher(EffectiveCPP::varUseMatcherEC, &Printer048);

    //Rule057 && Rule058  MatchCMemFunc
    CodStd101::CallExprPrinter Printer049;
    Finder.addMatcher(CodStd101::mallocMatcherCS, &Printer048);
    Finder.addMatcher(CodStd101::freeMatcherCS, &Printer048);
    Finder.addMatcher(CodStd101::reallocMatcherCS, &Printer048);
    Finder.addMatcher(CodStd101::memcpyAryMatcherCS, &Printer048);
    Finder.addMatcher(CodStd101::memcpyPtrMatcherCS, &Printer048);
    Finder.addMatcher(CodStd101::memcmpAryMatcherCS, &Printer048);
    Finder.addMatcher(CodStd101::memcmpPtrMatcherCS, &Printer048);

    //Rule059 MatchPrivate
    EffectiveCPP::RecordPrinter Printer050;
    Finder.addMatcher(EffectiveCPP::recordDeclMatcherEC, &Printer050);

    //Rule060 MatchPointeMath
    ProgrammingLanguage::PtrMemOpPrinter Printer051;
    Finder.addMatcher(ProgrammingLanguage::binaryOperatorMatcherPL, &Printer051);

    //Rule062 MatchVirtualFunc
    CodStd101::PubVtlPrinter Printer052;
    Finder.addMatcher(CodStd101::pubVtlFuncMatcherCS, &Printer052);

    //Rule063 MatchScope
    ProgrammingLanguage::ScopeLinesPrinter Printer053;
    Finder.addMatcher(ProgrammingLanguage::scopeStmtMatcherPL, &Printer053);

    //Rule064 MatchUsing
    ProgrammingLanguage::UsingPrinter Printer054;
    Finder.addMatcher(ProgrammingLanguage::usingDeclMatcherPL, &Printer054);
        
	ClangParser parser;

	parser.parseAST("E:\\WinApp\\webserver\\include\\settings.h", Finder);
    return 0;
}
