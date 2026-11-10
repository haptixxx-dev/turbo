# Syntax

Turbo's syntax closely follows that of other scripting languages like lua, python and ruby.
Turbo however has some minor additions and changes which make it easier to navigate as a scripting language for modifying Project Friction and it's gameplay.

This documentation is being updates as development occurs, and will absolutely change in the future.

Turbo files should be created with the `.turbo` extension.

## Table of Contents

- [Comments](#comments)
- [Identifiers and Keywords](#identifiers-and-keywords)
- [Literals](#literals)
- [Variables and Constants](#variables-and-constants)
- [Types](#types)
- [Operators](#operators)
- [Expressions and Statements](#expressions-and-statements)
- [Blocks and Scope](#blocks-and-scope)
- [Control Flow](#control-flow)
- [Functions](#functions)
- [Collections](#collections)
- [Structures and Objects](#structures-and-objects)
- [Modules and Imports](#modules-and-imports)
- [Error Handling](#error-handling)
- [Game Integration](#game-integration)
- [Standard Library](#standard-library)
- [Grammar Reference](#grammar-reference)

---

## Comments

<!-- Single-line comment syntax, multi-line/block comments, doc comments (if any). -->

```turbo
// TODO
```

## Identifiers and Keywords

<!-- Naming rules (allowed characters, case sensitivity), naming conventions, and the full list of reserved words. -->

### Naming Rules

### Reserved Keywords

| Keyword | Purpose |
|---------|---------|
|         |         |

## Literals

<!-- How each literal kind is written. -->

### Numbers

<!-- Integers, floats, hex/binary/octal, underscores as separators, scientific notation. -->

```turbo
// TODO
```

### Strings

<!-- Quote styles, escape sequences, multi-line strings, string interpolation. -->

```turbo
// TODO
```

### Booleans

```turbo
// TODO
```

### Nil / Null

<!-- Name of the "no value" value and how it behaves. -->

```turbo
// TODO
```

## Variables and Constants

<!-- Declaration keyword(s), assignment, multiple assignment, constants, default values. -->

### Declaration

```turbo
// TODO
```

### Assignment

```turbo
// TODO
```

### Constants

```turbo
// TODO
```

## Types

<!-- Static vs dynamic typing, built-in type list, type annotations, type conversion/casting, type checking at runtime. -->

### Built-in Types

| Type | Description | Example |
|------|-------------|---------|
|      |             |         |

### Type Annotations

```turbo
// TODO
```

### Type Conversion

```turbo
// TODO
```

## Operators

<!-- Fill in each table. Note operand types and result types where relevant. -->

### Arithmetic

| Operator | Description | Example |
|----------|-------------|---------|
| `+`      |             |         |
| `-`      |             |         |
| `*`      |             |         |
| `/`      |             |         |
| `%`      |             |         |

### Comparison

| Operator | Description | Example |
|----------|-------------|---------|
| `==`     |             |         |
| `!=`     |             |         |
| `<`      |             |         |
| `>`      |             |         |
| `<=`     |             |         |
| `>=`     |             |         |

### Logical

| Operator | Description | Example |
|----------|-------------|---------|
|          |             |         |

### Assignment and Compound Assignment

| Operator | Description | Example |
|----------|-------------|---------|
| `=`      |             |         |
| `+=`     |             |         |

### Other Operators

<!-- String concatenation, length, range, membership, ternary, null-coalescing, etc. -->

| Operator | Description | Example |
|----------|-------------|---------|
|          |             |         |

### Precedence

<!-- Highest to lowest. -->

| Precedence | Operators | Associativity |
|------------|-----------|---------------|
| 1          |           |               |

## Expressions and Statements

<!-- What counts as an expression vs a statement, statement terminators (newline / semicolon), line continuation. -->

```turbo
// TODO
```

## Blocks and Scope

<!-- How blocks are delimited (braces, indentation, `do ... end`), lexical scoping rules, shadowing, global vs local. -->

```turbo
// TODO
```

## Control Flow

### If / Else

```turbo
// TODO
```

### Match / Switch

```turbo
// TODO
```

### While Loop

```turbo
// TODO
```

### For Loop

<!-- Numeric ranges and iteration over collections. -->

```turbo
// TODO
```

### Break and Continue

```turbo
// TODO
```

### Return

```turbo
// TODO
```

## Functions

### Declaration

```turbo
// TODO
```

### Parameters

<!-- Default values, variadic parameters, named arguments. -->

```turbo
// TODO
```

### Return Values

<!-- Single and multiple return values. -->

```turbo
// TODO
```

### Anonymous Functions / Lambdas

```turbo
// TODO
```

### Closures

```turbo
// TODO
```

## Collections

### Arrays / Lists

<!-- Literal syntax, indexing (0- or 1-based), slicing, common operations. -->

```turbo
// TODO
```

### Maps / Dictionaries / Tables

<!-- Literal syntax, key access, iteration. -->

```turbo
// TODO
```

### Other Collections

<!-- Sets, tuples, ranges, if supported. -->

```turbo
// TODO
```

## Structures and Objects

<!-- Classes, structs, or table-based objects. Fields, methods, constructors, inheritance or composition, `self`/`this`. -->

### Definition

```turbo
// TODO
```

### Instantiation

```turbo
// TODO
```

### Methods

```turbo
// TODO
```

### Inheritance / Composition

```turbo
// TODO
```

## Modules and Imports

<!-- File-as-module rules, import/require syntax, exporting, namespacing. -->

```turbo
// TODO
```

## Error Handling

<!-- Exceptions vs error values, try/catch, raising errors, assertions. -->

```turbo
// TODO
```

## Game Integration

<!-- The Turbo-specific additions mentioned in the intro: how a script hooks into the game. -->

### Entry Points

<!-- What the engine calls: on load, per tick, on event, etc. -->

```turbo
// TODO
```

### Events and Callbacks

```turbo
// TODO
```

### Engine Globals

<!-- Objects or functions injected into every script by the engine. -->

| Name | Description |
|------|-------------|
|      |             |

## Standard Library

<!-- Brief index of built-in modules. Link to a dedicated page per module if this grows. -->

| Module | Description |
|--------|-------------|
|        |             |

## Grammar Reference

<!-- Optional: formal grammar in EBNF for the whole language. -->

```ebnf
program = { statement } ;
```
