default := 'help'

all:
    just sub
    just llvm
    just cfg
    just build
    just test

# --- setup ---------------------------------------------------------------

help:
    @echo "Usage: just [recipe]"
    @echo ""
    @echo "Recipes:"
    @echo "  sub      Update git submodules (the LLVM clone)"
    @echo "  llvm     Build the LLVM submodule (slow first time)"
    @echo "  cfg      Configure Turbo (preset: dev)"
    @echo "  build    Build Turbo in parallel"
    @echo "  test     Run ctest"
    @echo ""
    @echo "Env vars:"
    @echo "  PRESET=cl just build     # use a different CMake preset"
    @echo "  JOBS=4 just build        # cap parallelism"

sub:
    git submodule update --init --recursive

# Build the LLVM submodule if missing, reconfigure if needed, then build.
# First full run is the slow one (tens of minutes).
llvm:
    test -d submodules/llvm || just sub
    cmake -S submodules/llvm/llvm -B submodules/llvm/build \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang \
        -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ \
        -DLLVM_ENABLE_PROJECTS="clang;lld" \
        -DLLVM_TARGETS_TO_BUILD=host \
        -DLLVM_EXTERNAL_LIT="$(command -v lit || true)" \
        -DLLVM_INCLUDE_EXAMPLES=OFF \
        -DLLVM_INCLUDE_BENCHMARKS=OFF \
        -DLLVM_INCLUDE_TESTS=OFF \
        -DLLVM_PARALLEL_LINK_JOBS={{ llvm_jobs }}
    cmake --build submodules/llvm/build -- -j{{ jobs }}

cfg:
    cmake --preset {{ preset }}

# --- Turbo ----------------------------------------------------------------

build:
    cmake --build build/{{ preset }} -- -j{{ jobs }}

test:
    ctest --preset {{ preset }}

# --- helpers ---------------------------------------------------------------

preset := env_var_or_default('PRESET', 'dev')

jobs := env_var_or_default('JOBS', num_cpus())

llvm_jobs := num_cpus()

# --- docs ------------------------------------------------------------------

build-docs:
    source ./.venv/bin/activate
    mkdocs build