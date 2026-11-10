#pragma once

#include <vector>

#include "turbo/lexer.hpp"

namespace turbo::test {

// Convenience: fully tokenize a snippet from a fresh lexer position.
std::vector<lexer::token> lex_all(std::string const& source);

} // namespace turbo::test