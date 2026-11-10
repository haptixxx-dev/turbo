#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "turbo/codegen.hpp"
#include "turbo/lexer.hpp"
#include "turbo/parser.hpp"

namespace {

[[noreturn]] void fail(char const* message) {
    std::cerr << "turbo: " << message << "\n";
    std::exit(1);
}

void print_usage() {
    std::cout <<
        "turbo — the Turbo language compiler\n"
        "\n"
        "usage:\n"
        "  turbo emit-ir <file|->     print LLVM IR (- reads stdin)\n"
        "  turbo compile <file> -o x   compile to a native binary (TODO)\n"
        "  turbo version               print the version\n";
}

std::string read_source(std::istream& in, char const* what) {
    if (!in) { fail(std::format("cannot read {}", what).c_str()); }
    return std::string(std::istreambuf_iterator<char>(in),
                       std::istreambuf_iterator<char>());
}

void emit_ir(std::filesystem::path const& path) {
    std::string source;
    if (path == "-") {
        source = read_source(std::cin, "<stdin>");
    } else {
        std::ifstream in(path);
        source = read_source(in, path.string().c_str());
    }

    turbo::lexer::lexer lex(source);
    std::vector<turbo::lexer::token> tokens;
    turbo::lexer::token tok;
    do {
        tok = lex.next();
        if (tok.kind == turbo::lexer::token_kind::eof) { break; }
        tokens.push_back(tok);
    } while (true);

    turbo::parser::parser parser(std::move(tokens));
    turbo::parser::node ast = parser.parse_expression();

    std::string ir =
        turbo::codegen::to_llvm(std::filesystem::path{"turbo-module"}, ast);
    std::cout << ir << "\n";
}

} // namespace

int main(int argc, char** argv) {
    namespace fs = std::filesystem;
    try {
        std::vector<std::string> args(argv + 1, argv + argc);
        if (args.empty()) { print_usage(); return 0; }

        if (args[0] == "version") { std::cout << "turbo 0.1.0\n"; return 0; }
        if (args[0] == "-h" || args[0] == "--help") { print_usage(); return 0; }

        if (args[0] == "emit-ir") {
            if (args.size() < 2) { fail("emit-ir needs a file"); }
            emit_ir(fs::path{args[1]});
            return 0;
        }
        if (args[0] == "compile") { fail("compile: native backend not wired yet"); }
        fail(std::format("unknown command '{}'", args[0]).c_str());
    } catch (std::exception const& e) {
        fail(e.what());
    }
    return 0;
}