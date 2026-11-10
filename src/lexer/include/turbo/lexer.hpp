#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace turbo::lexer {

enum class token_kind : std::uint8_t {
    eof,
    int_literal,
    identifier,
    plus,
    minus,
    star,
    slash,
    open_paren,
    close_paren,
};

struct token {
    token_kind kind;
    std::string text;
    std::size_t line;
    std::size_t col;
};

class lexer {
public:
    explicit lexer(std::string_view code) : code_(code) {}

    [[nodiscard]] token next() const;

private:
    std::size_t skip_ws() const;
    [[nodiscard]] bool is_at_end() const;
    [[nodiscard]] char peek() const;

    std::string code_;
    mutable std::size_t pos_{0};
};

} // namespace turbo::lexer