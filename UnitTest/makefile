CXX := clang++ 
LLVMCOMPONENTS := cppbackend
RTTIFLAG := -fno-rtti
LLVMCONFIG := /usr/lib/llvm-3.5/bin/llvm-config

CXXFLAGS := -std=c++11 -s -I$(shell $(LLVMCONFIG) --src-root)/tools/clang/include -I$(shell $(LLVMCONFIG) --obj-root)/tools/clang/include $(shell $(LLVMCONFIG) --cxxflags) $(RTTIFLAG)
LLVMLDFLAGS := $(shell $(LLVMCONFIG) --ldflags --libs $(LLVMCOMPONENTS))


SOURCES = \
		  MatchCString.cpp \
		  MatchArray.cpp \
		  MatchContainer.cpp \
		  MatchMainCatch.cpp \
		  MatchRealloc.cpp \
		  MatchNotPlainChar.cpp \
		  MatchSignedCast.cpp \
		  MatchMultiDeclarations.cpp \
		  MatchUninitVar.cpp \
		  MatchAuto.cpp \
		  MatchListInit.cpp \
		  MatchIntegerToChar.cpp \
		  MatchMultidimArrays.cpp \
		  MatchNull.cpp \
		  MatchRefParm.cpp \
		  MatchHandleClass.cpp \
		  MatchNakedNew.cpp \
		  MatchVoidPointer.cpp \
		  MatchNamespace.cpp \
		  MatchStruct.cpp \
		  MatchBitField.cpp \
		  MatchEnum.cpp \
		  MatchForStmt.cpp \
		  MatchDoStmt.cpp \
		  MatchGotoStmt.cpp \
		  MatchSTD.cpp \
		  MatchNarrowingConversion.cpp \
		  MatchNewExpr.cpp \
		  MatchLambda.cpp \
		  MatchDynamicCast.cpp \
		  MatchFunCast.cpp \
		  MatchFuncConstexpr.cpp \
		  MatchPassByValue.cpp \
		  MatchReturnWarning.cpp \
		  MatchAryAsPtr.cpp \
		  MatchOverloading.cpp \
		  MatchShouldBeConstRef.cpp \
		  MatchVarParm.cpp \
		  CheckCast.cpp \
		  CheckMagicNumber.cpp \
		  CheckFuncLines.cpp \
		  CheckGlobalVar.cpp \
		  CheckInline.cpp \
		  MatchReturn.cpp \
		  MatchUsing.cpp \
		  MatchScope.cpp \
		  MatchVirtualFunc.cpp \
		  MatchVirtualCallee.cpp \
		  MatchVarDefine.cpp \
		  MatchSimilarName.cpp \
		  MatchCMemFunc.cpp \
		  MatchPrivate.cpp \
		  MatchPointerMatch.cpp \
		  MatchOnlyDefaultConstructor.cpp \
		  MatchOneParmConstructor.cpp \

OBJECTS = $(SOURCES:.cpp=.o)
EXES = $(OBJECTS:.o=)
CLANGLIBS = \
				-lclangTooling\
				-lclangFrontendTool\
				-lclangFrontend\
				-lclangDriver\
				-lclangSerialization\
				-lclangCodeGen\
				-lclangParse\
				-lclangSema\
				-lclangStaticAnalyzerFrontend\
				-lclangStaticAnalyzerCheckers\
				-lclangStaticAnalyzerCore\
				-lclangAnalysis\
				-lclangARCMigrate\
				-lclangRewriteFrontend\
				-lclangRewriteCore\
				-lclangEdit\
				-lclangAST\
				-lclangLex\
				-lclangBasic\
				-lclangASTMatchers \
				-lclangDynamicASTMatchers \
				$(shell $(LLVMCONFIG) --libs)\
				-lpthread \
				-lffi \
				-ltinfo \
				-ldl \
				-lm
                #-lcurses

all: $(OBJECTS) $(EXES)

%: %.o
	$(CXX) -o $@ $< $(CLANGLIBS) $(LLVMLDFLAGS)

.PHONY: clean
clean:
	-rm $(OBJECTS) $(EXES)
