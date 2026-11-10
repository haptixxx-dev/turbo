#include "turbo/parser.hpp"

#include <format>
#include <stdexcept>

namespace turbo::parser {

node parser::binary(node::node_kind k, node lhs, node rhs) {
    node out;
    out.kind = k;
    out.children = {std::move(lhs), std::move(rhs)};
    return out;
}

lexer::token parser::peek() const {
    if (pos_ >= tokens_.size()) {
        return {lexer::token_kind::eof, {}, 0, 0};
    }
    return tokens_[pos_];
}

lexer::token parser::advance() {
    lexer::token t = peek();
    if (t.kind != lexer::token_kind::eof) { pos_++; }
    return t;
}

bool parser::check(lexer::token_kind kind) const {
    return peek().kind == kind;
}

void parser::expect(lexer::token_kind kind) {
    lexer::token t = advance();
    if (t.kind != kind) {
        throw std::runtime_error(
            std::format("expected token, got '{}'", t.text));
    }
}

node parser::parse_primary() {
    lexer::token t = advance();
    node out;
    switch (t.kind) {
        case lexer::token_kind::int_literal:
            out.kind = node::node_kind::integer;
            out.value = std::stoll(t.text);
            return out;
        case lexer::token_kind::identifier:
            out.kind = node::node_kind::var;
            out.name = std::move(t.text);
            return out;
        case lexer::token_kind::eof:
        case lexer::token_kind::plus:
        case lexer::token_kind::minus:
        case lexer::token_kind::star:
        case lexer::token_kind::slash:
        case lexer::token_kind::open_paren:
        case lexer::token_kind::close_paren:
            throw std::runtime_error(
                std::format("expected expression, got '{}'", t.text));
    }
}

node parser::parse_factor() {
    if (check(lexer::token_kind::minus)) {
        (void)advance();
        node operand = parse_factor();
        node zero;
        zero.kind = node::node_kind::integer;
        zero.value = 0;
        return binary(node::node_kind::sub, std::move(zero), std::move(operand));
    }
    if (check(lexer::token_kind::open_paren)) {
        (void)advance();
        node inner = parse_expression();
        expect(lexer::token_kind::close_paren);
        return inner;
    }
    return parse_primary();
}

node parser::parse_term() {
    node lhs = parse_factor();
    while (check(lexer::token_kind::star) ||
           check(lexer::token_kind::slash)) {
        lexer::token_kind op = advance().kind;
        node rhs = parse_factor();
        lhs = binary(op == lexer::token_kind::star ? node::node_kind::mul
                                                   : node::node_kind::div,
                     std::move(lhs), std::move(rhs));
    }
    return lhs;
}

node parser::parse_expression() {
    node lhs = parse_term();
    while (check(lexer::token_kind::plus) ||
           check(lexer::token_kind::minus)) {
        lexer::token_kind op = advance().kind;
        node rhs = parse_term();
        lhs = binary(op == lexer::token_kind::plus ? node::node_kind::add
                                                   : node::node_kind::sub,
                     std::move(lhs), std::move(rhs));
    }
    return lhs;
}

} // namespace turbo::parser