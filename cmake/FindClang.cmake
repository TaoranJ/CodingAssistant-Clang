# Find CLANG: Find the CLANG includes and library (and the LLVM)
# (c) Copyright 2014. Cenacle Research India Private Limited
# License: BSD / MIT
#
# This module defines
#  CLANG_INCLUDE_DIRS, the include directories (includes the LLVM include directories also)
#  CLANG_LIBRARIES, the libraries needed to use CLANG.
#  CLANG_FOUND, If false, do not try to use CLANG.

FIND_PACKAGE(LLVM REQUIRED)

macro(Find_Clang_Library LibName)
	find_library(CLANG_${LibName}_LIB ${LibName} ${LLVM_LIBRARY_DIRS})
	if(CLANG_${LibName}_LIB)
	  set(CLANG_LIBRARIES ${CLANG_LIBRARIES} ${CLANG_${LibName}_LIB})
	endif()
endmacro(Find_Clang_Library)

Find_Clang_Library(clangAnalysis)
Find_Clang_Library(clangARCMigrate)
Find_Clang_Library(clangAST)
Find_Clang_Library(clangASTMatchers)
Find_Clang_Library(clangBasic)
Find_Clang_Library(clangCodeGen)
Find_Clang_Library(clangDriver)
Find_Clang_Library(clangDynamicASTMatchers)
Find_Clang_Library(clangEdit)
Find_Clang_Library(clangFormat)
Find_Clang_Library(clangFrontend)
Find_Clang_Library(clangFrontendTool)
Find_Clang_Library(clangIndex)
Find_Clang_Library(clangLex)
Find_Clang_Library(clangParse)
Find_Clang_Library(clangRewriteCore)
Find_Clang_Library(clangRewriteFrontend)
Find_Clang_Library(clangSema)
Find_Clang_Library(clangSerialization)
Find_Clang_Library(clangStaticAnalyzerCheckers)
Find_Clang_Library(clangStaticAnalyzerCore)
Find_Clang_Library(clangStaticAnalyzerFrontend)
Find_Clang_Library(clangTooling)

find_path(CLANG_BUILD_INCLUDE_DIR NAMES clang/Basic/Version.inc HINTS ${LLVM_OBJROOT_DIR}/tools/clang/include)
find_path(CLANG_SRC_INCLUDE_DIR NAMES clang/Basic/Version.h HINTS ${LLVM_SRC_INCLUDE_DIR}/../tools/clang/include)
set(CLANG_INCLUDE_DIRS ${CLANG_SRC_INCLUDE_DIR} ${CLANG_BUILD_INCLUDE_DIR} )

if(CLANG_LIBRARIES)
  set(CLANG_LIBRARIES ${CLANG_LIBRARIES} ${LLVM_LIBRARIES})
else()
  message(FATAL_ERROR "Unable to find CLANG_LIBRARIES")
endif()

if(CLANG_INCLUDE_DIRS)
  set(CLANG_FOUND TRUE)
  set(CLANG_INCLUDE_DIRS ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
  message(STATUS "Found Clang Include Dirs: ${CLANG_INCLUDE_DIRS}") 
else()
  message(FATAL_ERROR "Unable to find CLANG_INCLUDE_DIRS")
endif()

#
# Linking against CLANG_LIBRARIES is VERY late process
# this helper method helps reduce the latency by giving only the paths for used-libraries
# Usage:
#   Get_Clang_Lib_Paths(my_exe_Dependencies usedLib1 usedLib2)
#
macro(Get_Clang_Lib_Paths LibPathsOut)
  set(LibNamesIn ${ARGN}) # consider the optional arguments at the end as input libraries to filter on
  list(LENGTH LibNamesIn num_Input_LibNames)
  if(${num_Input_LibNames} GREATER 0)
    foreach(LibName ${LibNamesIn})
      set(libPath ${CLANG_${LibName}_LIB})
      if(libPath)
        LIST(APPEND ${LibPathsOut} ${libPath})
      else()
        message("[Get_Clang_Lib_Paths] Unknown Library: ${LibName}")
      endif()
    endforeach()
  else()
    set(${LibPathsOut} ${CLANG_LIBRARIES}) # no libraries specified on input, return all
  endif()
endmacro()