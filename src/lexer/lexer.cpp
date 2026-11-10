#include "turbo/lexer.hpp"

#include <cctype>
#include <format>
#include <stdexcept>

namespace turbo::lexer {

namespace {

token_kind kind_of_op(char c) {
    switch (c) {
        case '+': return token_kind::plus;
        case '-': return token_kind::minus;
        case '*': return token_kind::star;
        case '/': return token_kind::slash;
        case '(': return token_kind::open_paren;
        case ')': return token_kind::close_paren;
        default: return token_kind::eof;
    }
}

} // namespace

std::size_t lexer::skip_ws() const {
    while (pos_ < code_.size()) {
        if (std::isspace(static_cast<unsigned char>(code_[pos_]))) { pos_++; }
        else { break; }
    }
    return pos_;
}

bool lexer::is_at_end() const { return pos_ >= code_.size(); }

char lexer::peek() const {
    if (is_at_end()) { return 0; }
    return code_[pos_];
}

token lexer::next() const {
    skip_ws();
    if (is_at_end()) {
        return {token_kind::eof, {}, 0, pos_ + 1};
    }
    char c = code_[pos_];
    std::size_t start = pos_;

    if (std::isdigit(static_cast<unsigned char>(c))) {
        while (!is_at_end() &&
               std::isdigit(static_cast<unsigned char>(peek()))) {
            pos_++;
        }
        return {token_kind::int_literal, code_.substr(start, pos_ - start),
                1, start + 1};
    }
    if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
        while (!is_at_end() &&
               (std::isalnum(static_cast<unsigned char>(peek())) ||
                peek() == '_')) {
            pos_++;
        }
        return {token_kind::identifier, code_.substr(start, pos_ - start),
                1, start + 1};
    }
    token_kind kind = kind_of_op(c);
    if (kind == token_kind::eof) {
        throw std::runtime_error(
            std::format("unexpected character '{}'", c));
    }
    pos_++;
    return {kind, std::string(1, c), 1, start + 1};
}

} // namespace turbo::lexer