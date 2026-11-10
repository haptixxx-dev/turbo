add_library(turbo_codegen STATIC codegen.cpp)
target_include_directories(turbo_codegen PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(turbo_codegen PRIVATE turbo_parser turbo_warnings)