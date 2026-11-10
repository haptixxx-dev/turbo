add_library(turbo_parser STATIC parser.cpp)
target_include_directories(turbo_parser PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
# turbo_lexer is PUBLIC: parser.hpp names lexer::token and includes
# turbo/lexer.hpp, so consumers (codegen, tests) need the lexer include dir.
target_link_libraries(turbo_parser PUBLIC turbo_lexer PRIVATE turbo_warnings)