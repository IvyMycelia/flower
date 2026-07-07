# Flower Roadmap

Where Flower is going. Subject to change based on feedback, experiments, and progression.

## v1.1.0 (Stability)

Focus: Polish the working compiler.

- [x] Error Recovery                : Continue parsing after first error
- [x] Better Error Messages         : Context, suggestions, etc
- [x] Better Type-Tracking System   : Import handling / separation, proper type / pointer aware codegen

## v1.1.1 (Function Syntax)

Focus: Clean up core language syntax before larger features.

- [x] Replace old function syntax with `func name(...): type`
- [x] Support `prop func name(...): type`
- [x] Update parser / examples / tests / EVERYTHING
- [x] Remove legacy function syntax

## v1.2.0 (Modules & Interfaces)

Focus: Make imports and public APIs mean something semantically.

- [x] Top-level exports via `prop`
- [x] Imported module aliases act as real namespaces
- [x] Private-by-default top-level declarations
- [x] Field flags groundwork: `hidden`, `readonly`, `frozen`
- [x] Typecheck enforcement for external field access

*`frozen` is not fully implemented yet, but it exists.*

## v1.3.0 (Strings & Bools)

Core feature: Proper String and Boolean support.

- [x] Real `string` type support
- [x] Boolean support distinct from `1` and `0`
- [x] String operations
- [x] Examples and tests
- [x] Standard library basics

**Why String Operations and Booleans first?** Many programs need them. Current work arounds are verbose.

Current shipped scope:

- `bool` is a real semantic type, lwoered to integer output in the C backend
- `string` supports equality / inequality, `.length`, indexing, and explicit casts to / from `@char`
- `src/stdlib/string.flo` currently provides `is_empty`, `starts_with`, `ends_with`, and `find_char`

## v1.4.0 (Better Types)

Focus: Make Flower's type system more expressive while keeping semantics explicit and compiler-owned.

- [x] Transparent type aliases: `type Name = ExistingType`
- [x] Built-in `null` as a real language value
- [x] Nullable sugar: `?T == T | null`
- [x] Semantic unions: `A | B | C`
- [x] Explicit narrowing with `is`
- [x] Explicit extraction with `as`
- [x] Semantic union support in locals, parameters, return types, and struct fields
- [x] Field-expression narrowing on stable dot chains
- [x] Nullable field-expression narrowing on stable dot chains
- [x] Better inference in narrow, obvious contexts

Current v1.4 scope notes:

- Semantic unions are exprimental, but they are a real Flower-level semantics raher than C-only
- `union` remains the raw storage / layout construct; `A | B` is the semantic union construct
- Flower still requires explicit `as` after narrowing
- Narrowing on call results and subscript expressions is intentionally deferred for now

## v1.5.0 (Documentation)

Focus: Turn Flower's current behavior into a coherent reference.

- [x] Rewrite `docs/structure.md` into a current language surface document
- [ ] Write a real type-system reference for `type`, `null`, `?T`, `A | B`, `is`, and `as`
- [ ] Document module / import / export / visibility semantics clearly
- [ ] Document control flow and block structure clearly
- [ ] Document memory and ownership basics
- [ ] Audit README / ROADMAP / Milestone docs for drift
- [ ] Expand examples into reference-quality spec examples

## v1.6.0 (Standard Library)

Build out what is needed — stop relying on C backend.

- [ ] File I/O (read, write, open, close)
- [ ] Array Utilities (length, slice, sort)
- [ ] Math functions (sqrt, sin, cos, etc)
- [ ] String utilities (trim, split, replace)

## v1.7.0+ (Experiments)

- **Generics**: `<T> T max(a: T, b: T)`
- **Pattern Matching**: Better than `if` / `else`
- **Immutability by Explicity**: `x: int` vs `const x: int`
- **Operator Overloading**: Explicit overloads for operators on custom types
- **Defer Statement**: Cleanup code without explicit `prune` at end of scope
- **Error Types**: Structured return types for success / failureresults (e.g. Result-like enums)
- **Intrfaces**: Define contracts without inheritance or OOP hierarchies

## v2.0.0 (Non-C Codegen)

Rewriting the Flower Codegen to output to either Assembly, Binary, IR, or other machine code instead of C; Direct compilation.

- **Decide Target**: Assembly, Binary, IR, ByteCode, etc
- **Rewrite Codegen**: Instead of outputting to a .c file, output to .o or other
- **Optimization**: Short-circuit evalutation, pruning dead-branches, etc

## Non-Goals

Things Flower probably *won't* do:

- **Garbage Collection**: Manual memory exists; use it.
- **Object-Oriented**: No classes, inheritance, or virtual methods. Use Structs and Functions.
- **Implicit Conversion**: Be explicit. `x: int = 5.0 as int`
- **Macros**: Keep it simple. Generate C if you need metaprogramming.
- ~~**Module System Beyond Imports**: Current system works, no overengineering.~~
- **Heavy OOP-Style Type Systems**: No classes or method-dispath model just to support data access or modules

## How to Help

### Testing

- Use the current version, find bugs, report issues, test edge cases
- Benchmark and optimize
- Test on different platforms (MacOS, Linux, Windows, Embedded)

### New Documentation

- Write examples
- Clarify confusing parts of the spec
- Create tutorials

### Experimentation

- Branch, try ideas, report results
- Push boundaries
- Help decide what belongs in Flower

### Standard Library

- Implement missing functions
- Write tests for stdlib
- Optimize common operations

## How Priority is Decided

1. **Blocking Issues**: Bugs that prevent normal use
2. **Language Gaps**: Things you can't express properly yet
3. **Developer Experience**: Error messages, warnings, tooling
4. **Performance**: Optimizations that help real programs
5. **Nice to Have**: Everything else

## Feedback

This roadmap is a guide, not gospel. As Flower grows, priorities shift. Input you share helps shape its direction.

- Open an issue with ideas
- Comment on proposed ideas
- Show off what you've made with Flower

That's what determines what matters.
