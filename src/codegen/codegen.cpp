#include "turbo/codegen.hpp"

#include <format>
#include <stdexcept>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

namespace turbo::codegen {

namespace {

/// @brief Lower a Turbo AST node to an LLVM IR value.
/// @param b The IR builder.
/// @param n The Turbo AST node.
/// @return The corresponding LLVM IR value.
llvm::Value* lower(llvm::IRBuilder<> &b, turbo::parser::node const& n) {
    using turbo::parser::node;
    switch (n.kind) {
        case node::node_kind::integer:
            return llvm::ConstantInt::get(
                llvm::Type::getInt64Ty(b.getContext()), n.value);
        case node::node_kind::var:
            // Variables are globals: i64 @name
            return b.GetInsertBlock()->getParent()->getParent()
                ->getGlobalVariable(n.name);
        case node::node_kind::add:
            return b.CreateAdd(lower(b, n.children[0]), lower(b, n.children[1]));
        case node::node_kind::sub:
            return b.CreateSub(lower(b, n.children[0]), lower(b, n.children[1]));
        case node::node_kind::mul:
            return b.CreateMul(lower(b, n.children[0]), lower(b, n.children[1]));
        case node::node_kind::div:
            return b.CreateSDiv(lower(b, n.children[0]), lower(b, n.children[1]));
    }
    throw std::runtime_error("codegen: unreachable node kind");
}

} // namespace

/// @brief Lower a Turbo AST to an LLVM IR string.
/// @param module_name The name of the LLVM module.
/// @param ast The Turbo AST to lower.
/// @return The corresponding LLVM IR string.
std::string to_llvm(std::string const& module_name,
                    turbo::parser::node const& ast) {
    llvm::LLVMContext ctx;
    llvm::Module mod(module_name, ctx);
    llvm::IRBuilder<> b(mod);

    llvm::FunctionType* fn_type =
        llvm::FunctionType::get(llvm::Type::getInt64Ty(ctx), {}, false);
    llvm::Function* fn = llvm::Function::Create(
        fn_type, llvm::Function::ExternalLinkage, "entry", &mod);
    llvm::BasicBlock* entry_block =
        llvm::BasicBlock::Create(ctx, "entry", fn);
    b.SetInsertPoint(entry_block);

    llvm::Value* result = lower(b, ast);
    b.CreateRet(result);

    std::string out;
    llvm::raw_string_ostream out_stream(out);
    mod.print(out_stream, nullptr);
    out_stream.flush();
    return out;
}

} // namespace turbo::codegen