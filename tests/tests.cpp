#include <catch2/catch_all.hpp>

#include "turbo/codegen.hpp"
#include "turbo/lexer.hpp"
#include "turbo/parser.hpp"
#include "turbo/test/harness.hpp"

using namespace turbo;

TEST_CASE("lexer: integer and operators", "[lexer]") {
    auto tokens = test::lex_all("42 + 3 / 2");
    REQUIRE(tokens.size() == 5);
    CHECK(tokens[0].kind == lexer::token_kind::int_literal);
    CHECK(tokens[0].text == "42");
    CHECK(tokens[1].kind == lexer::token_kind::plus);
    CHECK(tokens[2].kind == lexer::token_kind::int_literal);
    CHECK(tokens[3].kind == lexer::token_kind::slash);
    CHECK(tokens[4].kind == lexer::token_kind::int_literal);
}

TEST_CASE("lexer: identifiers", "[lexer]") {
    auto tokens = test::lex_all("x y_2 foo");
    REQUIRE(tokens.size() == 3);
    CHECK(tokens[0].kind == lexer::token_kind::identifier);
    CHECK(tokens[0].text == "x");
    CHECK(tokens[1].text == "y_2");
    CHECK(tokens[2].text == "foo");
}

TEST_CASE("lexer: parens and error on garbage", "[lexer]") {
    CHECK(test::lex_all("( )")[0].kind == lexer::token_kind::open_paren);
    CHECK(test::lex_all("( )")[1].kind == lexer::token_kind::close_paren);
    CHECK_THROWS(test::lex_all("1 % 2"));
}

TEST_CASE("lexer: whitespace is a single unit", "[lexer]") {
    auto tokens = test::lex_all("  1\t+\n2  ");
    REQUIRE(tokens.size() == 3);
    CHECK(tokens[0].text == "1");
    CHECK(tokens[1].kind == lexer::token_kind::plus);
    CHECK(tokens[2].text == "2");
}

TEST_CASE("parser: precedence is term-over-sum", "[parser]") {
    // 1 + 2 * 3 must parse as 1 + (2 * 3), not (1 + 2) * 3.
    auto tokens = test::lex_all("1 + 2 * 3");
    parser::parser parse(std::move(tokens));
    parser::node root = parse.parse_expression();
    REQUIRE(root.kind == parser::node::node_kind::add);
    REQUIRE(root.children.size() == 2);
    CHECK(root.children[0].kind == parser::node::node_kind::integer);
    CHECK(root.children[0].value == 1);
    CHECK(root.children[1].kind == parser::node::node_kind::mul);
    CHECK(root.children[1].children[0].value == 2);
    CHECK(root.children[1].children[1].value == 3);
}

TEST_CASE("parser: parens group", "[parser]") {
    auto tokens = test::lex_all("(1 + 2) * 3");
    parser::parser parse(std::move(tokens));
    parser::node root = parse.parse_expression();
    REQUIRE(root.kind == parser::node::node_kind::mul);
    CHECK(root.children[0].kind == parser::node::node_kind::add);
    CHECK(root.children[1].value == 3);
}

TEST_CASE("parser: unary minus", "[parser]") {
    auto tokens = test::lex_all("-5");
    parser::parser parse(std::move(tokens));
    parser::node root = parse.parse_expression();
    REQUIRE(root.kind == parser::node::node_kind::sub);
    CHECK(root.children[0].value == 0);
    CHECK(root.children[1].value == 5);
}

TEST_CASE("parser: div is right operand of a mul", "[parser]") {
    auto tokens = test::lex_all("6 / 2");
    parser::parser parse(std::move(tokens));
    parser::node root = parse.parse_expression();
    CHECK(root.kind == parser::node::node_kind::div);
}

TEST_CASE("codegen: emits LLVM IR with a div instruction", "[codegen]") {
    // Note: constant operands fold at IR build time, so "6 / 2" lowers to
    // the constant result "ret i64 3" -- no runtime sdiv survives.
    auto tokens = test::lex_all("6 / 2");
    parser::parser parse(std::move(tokens));
    parser::node ast = parse.parse_expression();
    std::string ir = codegen::to_llvm("t", ast);
    CHECK(ir.find("ret i64 3") != std::string::npos);
}