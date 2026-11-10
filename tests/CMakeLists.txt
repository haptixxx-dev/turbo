add_executable(turbo_tests tests.cpp harness.cpp)
target_include_directories(turbo_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Catch2 v3 arrives via FetchContent (pinned tag = reproducible).
# The LLVM component libs are already exposed as a plain variable from the
# root; link them here because codegen pulls in llvm/IR headers.
target_link_libraries(turbo_tests PRIVATE
    turbo_lexer turbo_parser turbo_codegen
    turbo_warnings
    ${turbo_llvm_libs})

include(FetchContent)
find_package(Catch2 3 QUIET)
if(NOT Catch2_FOUND)
  FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.0
    GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(Catch2)
endif()
target_link_libraries(turbo_tests PRIVATE Catch2::Catch2WithMain)

add_test(NAME turbo_tests COMMAND turbo_tests)
set_tests_properties(turbo_tests PROPERTIES LABELS "frontend")