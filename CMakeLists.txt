cmake_minimum_required(VERSION 3.31)
# C is enabled because LLVMConfig's FindLibEdit runs a C try_compile probe.
project(turbo VERSION 0.1.0 LANGUAGES C CXX)

set(TURBO_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Pick Clang before project() has run.
# Homebrew clang (23.x) has full C++23 library coverage; Xcode's Apple Clang lags.
if(NOT CMAKE_CXX_COMPILER AND EXISTS "/opt/homebrew/opt/llvm/bin/clang++")
  set(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Release. Turbo is a compiler: slow code is the enemy.
# Override per-configure with -DCMAKE_BUILD_TYPE=Debug if needed.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

include(CTest)

add_library(turbo_warnings INTERFACE)
target_compile_options(turbo_warnings INTERFACE
    -Wall -Wextra -Wpedantic -Werror -Wswitch-enum
    # __COUNTER__ is a GNU extension used inside Catch2 macros; Clang 23
    # diagnoses it under C++23. Keep the test suite free of vendor macro noise.
    -Wno-c2y-extensions)

# --- Fast linker -----------------------------------------------------------
# mold is ELF-only, so on macOS the fast choice is ld64.lld (lld's Mach-O).
# The compiler driver is forced to it via a shim directory on PATH that
# holds an `ld` wrapper calling the real linker, so all link steps (including
# CMake's own) use it.
if(APPLE)
  find_program(TURBO_LINKER ld64.lld)
  if(NOT TURBO_LINKER)
    find_program(TURBO_LINKER mold)
  endif()
else()
  find_program(TURBO_LINKER mold)
  if(NOT TURBO_LINKER)
    find_program(TURBO_LINKER ld.lld)
  endif()
endif()

if(TURBO_LINKER)
  message(STATUS "Turbo fast linker: ${TURBO_LINKER}")
  set(TURBO_LINK_SHIM_DIR ${CMAKE_BINARY_DIR}/linker-shim)
  file(MAKE_DIRECTORY ${TURBO_LINK_SHIM_DIR})
  file(WRITE ${TURBO_LINK_SHIM_DIR}/ld
       "${TURBO_LINKER} $@")
  file(CHMOD ${TURBO_LINK_SHIM_DIR}/ld PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
  set(CMAKE_PROGRAM_PATH ${TURBO_LINK_SHIM_DIR} ${CMAKE_PROGRAM_PATH})
else()
  message(WARNING
      "Turbo could not find a fast linker (mold or ld64.lld). "
      "Use the system linker; linking will be slower. "
      "Install one: Linux: mold; macOS: brew install lld.")
endif()

# --- LLVM (submodule at submodules/llvm) ----------------------------------
# Configure LLVM itself (out-of-tree, its own binary dir) BEFORE configuring
# Turbo:
#   cmake -S submodules/llvm/llvm -B submodules/llvm/build
#         -G Ninja -DCMAKE_BUILD_TYPE=Release
#         -DLLVM_ENABLE_PROJECTS="clang;lld"
#         -DLLVM_TARGETS_TO_BUILD=host -DCMAKE_C_COMPILER=clang
#         -DCMAKE_CXX_COMPILER=clang++
#         -DLLVM_EXTERNAL_LIT="$(command -v lit || true)"
#   cmake --build submodules/llvm/build
# The dev preset points LLVM_DIR at the standard build location for the rest.
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# SYSTEM keeps -I ordering above the C++ standard library and silences
# diagnostics from LLVM's vendored headers.
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})

# NOTE: the CMake variables LLVM_DEFINITIONS / LLVM_CONFIG_ADDITIONAL_CXXFLAGS
# include Homebrew-specific flags (-isystem $SDK/usr/include, -isystem
# /opt/homebrew/include). Adding those to the command line breaks the
# include-order that libc++ needs (its <cmath>/cerrno then find the SDK C
# headers first and 18 errors cascade). Homebrew clang already carries the
# equivalent defines, so do not add_definitions(${LLVM_DEFINITIONS_LIST}).

# The libraries the frontend links now. Add components here as real modules
# need them (analysis, passes, mc, target backends).
llvm_map_components_to_libnames(turbo_llvm_libs
    support core irreader mcjit)

add_subdirectory(src/lexer)
add_subdirectory(src/parser)
add_subdirectory(src/codegen)

add_executable(turbo src/main.cpp)
target_link_libraries(turbo PRIVATE
    turbo_lexer turbo_parser turbo_codegen ${turbo_llvm_libs}
    turbo_warnings)

add_subdirectory(tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_C_COMPILER MATCHES "CC$|clang")
  # Apple Clang lags C++23 library coverage; make sure we are not on it.
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} --version
      OUTPUT_VARIABLE turbo_compiler_version)
  string(REGEX MATCH "Apple clang version [0-9.]+"
      turbo_apple_match "${turbo_compiler_version}")
  if(turbo_apple_match)
    message(WARNING
        "Turbo is building with ${turbo_apple_match}. "
        "Apple Clang C++23 library coverage is incomplete. "
        "Use Homebrew clang (brew install llvm) for full coverage.")
  endif()
endif()