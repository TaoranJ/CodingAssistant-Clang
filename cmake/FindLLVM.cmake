# Find LLVM: Find the LLVM includes and library
# (c) Copyright 2014. Cenacle Research India Private Limited
# License: BSD / MIT
#
# This module defines
#  LLVM_INCLUDE_DIRS, the include directories
#  LLVM_LIBRARY_DIRS, the library directories
#  LLVM_LIBRARIES, the libraries needed to use LLVM.
#  LLVM_OBJROOT_DIR, the root build directory used for LLVM
#  LLVM_SRC_INCLUDE_DIR, the source include directory of LLVM
#  LLVM_BUILD_INCLUDE_DIR, the build include directory of LLVM
#  LLVM_CPPFLAGS, pre-processor flags for files that include LLVM
#  LLVM_FOUND, If false, do not try to use LLVM.

macro(Get_LLVM_Lib_Paths LibPathsOut)
  set(ComponentsIn ${ARGN}) # consider the optional arguments at the end as input components to filter on

  execute_process(COMMAND ${LLVM_CONFIG_EXE} --libfiles ${ComponentsIn} OUTPUT_VARIABLE ${LibPathsOut} OUTPUT_STRIP_TRAILING_WHITESPACE)

  # --libfiles option gives space separated values - we need ; separated values
  string(REGEX REPLACE " +" ";" ${LibPathsOut} ${${LibPathsOut}})

  # message(STATUS "before: ${LibPathsOut}=${${LibPathsOut}}")

  # On Win32 for some reason llvm-config is giving unix style names. Lets correct them
  if(WIN32)
      string(REGEX REPLACE "lib([A-Za-z0-9_]+).a" "\\1.lib;" ${LibPathsOut} ${${LibPathsOut}})
  endif()

  # message(STATUS "after: ${LibPathsOut}=${${LibPathsOut}}")   
endmacro()

# finding the llvm-config app helps us find everything else
find_program(LLVM_CONFIG_EXE NAMES llvm-config PATHS /opt/local/bin DOC "Path to the llvm-config executable")

if(LLVM_CONFIG_EXE)
 message(STATUS "LLVM-Config found at: ${LLVM_CONFIG_EXE}")
else()
 message(FATAL_ERROR "LLVM Config Not Found")
endif()

# llvm-config has options that give us all required details
# just execute the llvm-config and you are good to go
# no need for complex pkgconfig etc..
execute_process(COMMAND ${LLVM_CONFIG_EXE} --includedir OUTPUT_VARIABLE LLVM_SRC_INCLUDE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND ${LLVM_CONFIG_EXE} --cppflags OUTPUT_VARIABLE LLVM_CPPFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND ${LLVM_CONFIG_EXE} --libdir OUTPUT_VARIABLE LLVM_LIBRARY_DIRS OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND ${LLVM_CONFIG_EXE} --obj-root OUTPUT_VARIABLE LLVM_OBJROOT_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)

# execute_process(COMMAND ${LLVM_CONFIG_EXE} --system-libs OUTPUT_VARIABLE LLVM_SYS_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

set(LLVM_BUILD_INCLUDE_DIR ${LLVM_OBJROOT_DIR}/include)
set(LLVM_INCLUDE_DIRS ${LLVM_SRC_INCLUDE_DIR} ${LLVM_BUILD_INCLUDE_DIR})
Get_LLVM_Lib_Paths(LLVM_LIBRARIES)

if(LLVM_CONFIG_EXE)
 set(LLVM_FOUND TRUE)
 message(STATUS "Found LLVM Include Dirs: ${LLVM_INCLUDE_DIRS}")
 message(STATUS "Found LLVM Library Dirs: ${LLVM_LIBRARY_DIRS}")
endif()