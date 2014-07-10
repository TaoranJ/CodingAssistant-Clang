CXX := clang++ 
LLVMCOMPONENTS := cppbackend
RTTIFLAG := -fno-rtti
LLVMCONFIG := /usr/lib/llvm-3.5/bin/llvm-config

CXXFLAGS := -std=c++11 -s -I$(shell $(LLVMCONFIG) --src-root)/tools/clang/include -I$(shell $(LLVMCONFIG) --obj-root)/tools/clang/include $(shell $(LLVMCONFIG) --cxxflags) $(RTTIFLAG) 
LLVMLDFLAGS := $(shell $(LLVMCONFIG) --ldflags --libs $(LLVMCOMPONENTS))

OBJECTS = main.o \
		  MatchCString.o \
		  MatchArray.o \
		  MatchContainer.o \
		  MatchMainCatch.o \
		  MatchRealloc.o \
		  MatchNotPlainChar.o \
		  MatchSignedCast.o \
		  MatchMultiDeclarations.o \
		  MatchUninitVar.o \
		  MatchAuto.o \
		  MatchListInit.o \
		  MatchIntegerToChar.o \
		  MatchMultidimArrays.o \
		  MatchNull.o \
		  MatchRefParm.o \
		  MatchHandleClass.o \
		  MatchNakedNew.o \
		  MatchVoidPointer.o \
		  MatchStruct.o \
		  MatchBitField.o \
		  MatchEnum.o \
		  MatchForStmt.o \
		  MatchDoStmt.o \
		  MatchGotoStmt.o \
		  MatchSTD.o \
		  MatchNarrowingConversion.o \
		  MatchNewExpr.o \
		  MatchLambda.o \
		  MatchFunCast.o \
		  MatchDynamicCast.o \
		  MatchFuncConstexpr.o \
		  MatchShouldBeConstRef.o \
		  MatchPassByValue.o \
		  MatchReturnWarning.o \
		  MatchAryAsPtr.o \
		  MatchVarParm.o \
		  MatchOverloading.o \
		  MatchNamespace.o \
		  MatchCast.o \
		  MatchMagicNumber.o \
		  MatchFuncLines.o \
		  MatchGlobalVar.o \
		  MatchSimilarName.o \
		  MatchUninline.o \
		  MatchVirtualCallee.o \
		  MatchVarDefine.o \
		  MatchCMemFunc.o \
		  MatchPrivate.o \
		  MatchPointerMatch.o \
		  MatchVirtualFunc.o \
		  MatchScope.o \
		  MatchUsing.o \
		  MatchOnlyDefaultConstructor.o

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


utility: $(OBJECTS)
	$(CXX) -o $@ $^ $(CLANGLIBS) $(LLVMLDFLAGS) $(CXXFLAGS)

$(OBJECTS):%.o:%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean
clean:
	-rm $(OBJECTS) Rule* utility
