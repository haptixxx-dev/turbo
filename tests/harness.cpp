#include "turbo/test/harness.hpp"

namespace turbo::test {

std::vector<lexer::token> lex_all(std::string const& source) {
    lexer::lexer lex(source);
    std::vector<lexer::token> out;
    for (auto tok = lex.next(); tok.kind != lexer::token_kind::eof;
         tok = lex.next()) {
        out.push_back(tok);
    }
    return out;
}

} // namespace turbo::test