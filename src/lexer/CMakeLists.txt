add_library(turbo_lexer STATIC lexer.cpp)
target_include_directories(turbo_lexer PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(turbo_lexer PRIVATE turbo_warnings)