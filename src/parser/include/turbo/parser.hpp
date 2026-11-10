#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "turbo/lexer.hpp"

namespace turbo::parser {

struct node {
    enum class node_kind : std::uint8_t {
        integer,
        var,
        add,
        sub,
        mul,
        div,
    } kind;
    long long value{0}; // integer
    std::string name;   // var
    std::vector<node> children;
};

class parser {
public:
    explicit parser(std::vector<lexer::token> tokens)
        : tokens_(std::move(tokens)) {}

    [[nodiscard]] node parse_expression();

private:
    [[nodiscard]] node parse_term();
    [[nodiscard]] node parse_factor();
    [[nodiscard]] node parse_primary();
    [[nodiscard]] lexer::token peek() const;
    [[nodiscard]] lexer::token advance();
    [[nodiscard]] bool check(lexer::token_kind kind) const;
    void expect(lexer::token_kind kind);
    static node binary(node::node_kind k, node lhs, node rhs);

    std::vector<lexer::token> tokens_;
    std::size_t pos_{0};
};

} // namespace turbo::parser