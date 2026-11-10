<!-- markdownlint-disable MD033 MD041 -->
<div align="center">

<img src="assets/TurboLang-Icon.svg" alt="Turbo logo" width="160" />

# Turbo

**A compiled language used by Haptixxx games.**

[Documentation](docs/index.md) · [Syntax](docs/syntax.md) · [Grammar](docs/grammar.ebnf)

</div>
<!-- markdownlint-enable MD033 MD041 -->

---

## Getting started

Turbo is built on LLVM, which is included as a submodule. The first build is slow because it compiles LLVM.

```sh
just all     # submodules, LLVM, configure, build, test
```

Run `just` to list the individual recipes.

## Documentation

The docs are built with [MkDocs](https://www.mkdocs.org/). Start with the [syntax guide](docs/syntax.md).
