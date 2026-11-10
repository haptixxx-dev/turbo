#pragma once

#include <cstdint>
#include <string>

#include "turbo/parser.hpp"

namespace turbo::codegen {

/// @brief Lower a Turbo AST to an LLVM IR string.
/// @param module_name The name of the LLVM module.
/// @param ast The Turbo AST.
/// @return The corresponding LLVM IR string.
std::string to_llvm(std::string const& module_name,
                    parser::node const& ast);

} // namespace turbo::codegen