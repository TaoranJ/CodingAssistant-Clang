cmake_minimum_required(VERSION 2.8)

PROJECT( codingassistant )

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

Find_Package(Clang REQUIRED)

if(CMAKE_COMPILER_IS_GNUCC)
  SET(TARGET_COMPILE_FLAGS "-std=gnu++11") # Add C++11 support for GCC
endif()

set(TARGET_COMPILE_DEFS "_CRT_SECURE_NO_DEPRECATE;_CRT_SECURE_NO_WARNINGS;_BIND_TO_CURRENT_CRT_VERSION")

##########################################
# ------ Target: codingassistant ------------
set(CAProjDir ${CMAKE_CURRENT_SOURCE_DIR})

set(CA_Source_Files 
	  ${CAProjDir}/src/MatchCString.cpp 
	  ${CAProjDir}/src/MatchArray.cpp 
	  ${CAProjDir}/src/MatchContainer.cpp 
	  ${CAProjDir}/src/MatchMainCatch.cpp 
	  ${CAProjDir}/src/MatchRealloc.cpp 
	  ${CAProjDir}/src/MatchNotPlainChar.cpp 
	  ${CAProjDir}/src/MatchSignedCast.cpp 
	  ${CAProjDir}/src/MatchMultiDeclarations.cpp 
	  ${CAProjDir}/src/MatchUninitVar.cpp 
	  ${CAProjDir}/src/MatchAuto.cpp 
	  ${CAProjDir}/src/MatchListInit.cpp 
	  ${CAProjDir}/src/MatchIntegerToChar.cpp 
	  ${CAProjDir}/src/MatchMultidimArrays.cpp 
	  ${CAProjDir}/src/MatchNull.cpp 
	  ${CAProjDir}/src/MatchRefParm.cpp 
	  ${CAProjDir}/src/MatchHandleClass.cpp 
	  ${CAProjDir}/src/MatchNakedNew.cpp 
	  ${CAProjDir}/src/MatchVoidPointer.cpp 
	  ${CAProjDir}/src/MatchStruct.cpp 
	  ${CAProjDir}/src/MatchBitField.cpp 
	  ${CAProjDir}/src/MatchEnum.cpp 
	  ${CAProjDir}/src/MatchForStmt.cpp 
	  ${CAProjDir}/src/MatchDoStmt.cpp 
	  ${CAProjDir}/src/MatchGotoStmt.cpp 
	  ${CAProjDir}/src/MatchSTD.cpp 
	  ${CAProjDir}/src/MatchNarrowingConversion.cpp 
	  ${CAProjDir}/src/MatchNewExpr.cpp 
	  ${CAProjDir}/src/MatchLambda.cpp 
	  ${CAProjDir}/src/MatchFunCast.cpp 
	  ${CAProjDir}/src/MatchDynamicCast.cpp 
	  ${CAProjDir}/src/MatchFuncConstexpr.cpp 
	  ${CAProjDir}/src/MatchShouldBeConstRef.cpp 
	  ${CAProjDir}/src/MatchPassByValue.cpp 
	  ${CAProjDir}/src/MatchReturnWarning.cpp 
	  ${CAProjDir}/src/MatchAryAsPtr.cpp 
	  ${CAProjDir}/src/MatchVarParm.cpp 
	  ${CAProjDir}/src/MatchOverloading.cpp 
	  ${CAProjDir}/src/MatchNamespace.cpp 
	  ${CAProjDir}/src/MatchCast.cpp 
	  ${CAProjDir}/src/MatchMagicNumber.cpp 
	  ${CAProjDir}/src/MatchFuncLines.cpp 
	  ${CAProjDir}/src/MatchGlobalVar.cpp 
	  ${CAProjDir}/src/MatchSimilarName.cpp 
	  ${CAProjDir}/src/MatchUninline.cpp 
	  ${CAProjDir}/src/MatchVirtualCallee.cpp 
	  ${CAProjDir}/src/MatchVarDefine.cpp 
	  ${CAProjDir}/src/MatchCMemFunc.cpp 
	  ${CAProjDir}/src/MatchPrivate.cpp 
	  ${CAProjDir}/src/MatchPointerMatch.cpp 
	  ${CAProjDir}/src/MatchVirtualFunc.cpp 
	  ${CAProjDir}/src/MatchScope.cpp 
	  ${CAProjDir}/src/MatchUsing.cpp
	  ${CAProjDir}/src/main.cpp)
set(CA_Header_Files
	  ${CAProjDir}/include/ASTUtility.h
	  ${CAProjDir}/include/CodStd101.h
	  ${CAProjDir}/include/EffectiveCPP.h
	  ${CAProjDir}/include/MatchMultiDeclarations.h
	  ${CAProjDir}/include/MatchUninitVar.h
	  ${CAProjDir}/include/MatchAuto.h
	  ${CAProjDir}/include/MatchListInit.h
	  ${CAProjDir}/include/MatchIntegerToChar.h
	  ${CAProjDir}/include/MatchMultidimArrays.h
	  ${CAProjDir}/include/MatchNull.h
	  ${CAProjDir}/include/MatchRefParm.h
	  ${CAProjDir}/include/MatchHandleClass.h
	  ${CAProjDir}/include/MatchNakedNew.h
	  ${CAProjDir}/include/MatchVoidPointer.h
	  ${CAProjDir}/include/MatchStruct.h
	  ${CAProjDir}/include/MatchBitField.h
	  ${CAProjDir}/include/MatchEnum.h
	  ${CAProjDir}/include/MatchForStmt.h
	  ${CAProjDir}/include/MatchDoStmt.h
	  ${CAProjDir}/include/MatchGotoStmt.h
	  ${CAProjDir}/include/MatchSTD.h
	  ${CAProjDir}/include/MatchNarrowingConversion.h
	  ${CAProjDir}/include/MatchNewExpr.h
	  ${CAProjDir}/include/MatchLambda.h
	  ${CAProjDir}/include/MatchFunCast.h
	  ${CAProjDir}/include/MatchDynamicCast.h
	  ${CAProjDir}/include/MatchFuncConstexpr.h
	  ${CAProjDir}/include/MatchShouldBeConstRef.h
	  ${CAProjDir}/include/MatchPassByValue.h
	  ${CAProjDir}/include/MatchReturnWarning.h
	  ${CAProjDir}/include/MatchAryAsPtr.h
	  ${CAProjDir}/include/MatchVarParm.h
	  ${CAProjDir}/include/MatchOverloading.h
	  ${CAProjDir}/include/MatchNamespace.h
	  ${CAProjDir}/include/MatchCast.h
	  ${CAProjDir}/include/MatchMagicNumber.h
	  ${CAProjDir}/include/MatchFuncLines.h
	  ${CAProjDir}/include/MatchGlobalVar.h
	  ${CAProjDir}/include/MatchSimilarName.h
	  ${CAProjDir}/include/MatchUninline.h
	  ${CAProjDir}/include/MatchVirtualCallee.h
	  ${CAProjDir}/include/MatchVarDefine.h
	  ${CAProjDir}/include/MatchCMemFunc.h
	  ${CAProjDir}/include/MatchPrivate.h
	  ${CAProjDir}/include/MatchPointerMatch.h
	  ${CAProjDir}/include/MatchVirtualFunc.h
	  ${CAProjDir}/include/MatchScope.h
	  ${CAProjDir}/include/MatchUsing.h)

# we can use CLANG_LIBRARIES and LLVM_LIBRARIES instead. But this is little bit faster
Get_Clang_Lib_Paths(CLANG_LIB_PATHS clangAnalysis clangAST clangASTMatchers clangBasic clangCodeGen clangDriver clangEdit clangFrontend clangLex clangParse clangSema clangSerialization clangTooling )

Get_LLVM_Lib_Paths(LLVM_LIB_PATHS asmparser asmprinter bitreader bitwriter codegen core cppbackend cppbackendcodegen cppbackendinfo debuginfo engine executionengine instcombine instrumentation interpreter ipa ipo irreader jit lineeditor linker lto mc mcdisassembler mcjit mcparser native nativecodegen objcarcopts object option profiledata runtimedyld scalaropts selectiondag support tablegen target transformutils vectorize x86 x86asmparser x86asmprinter x86codegen x86desc x86disassembler x86info x86utils)

set(CA_Compile_Flags ${TARGET_COMPILE_FLAGS} ${LLVM_CPPFLAGS})
set(CA_Compile_Defs  ${TARGET_COMPILE_DEFS})
set(CA_Include_Dirs  ${CLANG_INCLUDE_DIRS})
set(CA_Dependencies  ${CLANG_LIB_PATHS} ${LLVM_LIB_PATHS})

add_executable(codingassistant 
	${CA_Source_Files}
	${CA_Header_Files})
set_target_properties(codingassistant PROPERTIES 
    COMPILE_DEFINITIONS "${CA_Compile_Defs}" 
    COMPILE_FLAGS       "${CA_Compile_Flags}"
    INCLUDE_DIRECTORIES "${CA_Include_Dirs}")
target_link_libraries(codingassistant ${CA_Dependencies})
