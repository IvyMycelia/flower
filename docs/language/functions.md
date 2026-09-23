# Functions

## What This Covers

This page documents Flower's current function surface.

That includes:

- function declarations
- exported functions via `prop func`
- parameters
- return types
- `return`
- function calls
- imported module alias calls like `math.add(...)`
- forward declarations via `forward func`
- how functions interact with aliases, nullable types, and semantic unions
- what function features do not exist yet

This page is about how functions work *today*, not how they might work in later versions of Flower.

## Current Surface

Flower currently supports these function-related forms:

### Plain Functions

```flo
func add(a: int, b: int): int
    return a + b
end
```

### Exported Functions

```flo
prop func test(): bool
    return true
end
```

### Forwards

```flo
forward func add(a: int, b: int): int
```

### Direct Calls

```flo
sum: int = add(1, 2)
```

### Module Alias Calls

```flo
import "examples/import/math.flo" as math

value: int = math.add(1, 2)
```

### System / Backend Imported Calls

These names come from system / backend imports and are backend-dependent; they are not ordinary Flower function declarations.

```flo
import <stdio>
import <stdlib>

printf("Hello, %s\n", "world")
name: @char = getenv("USER" as @char)
```

### Nullable and Union-Aware Functions

```flo
func pass_name(name: ?string): ?string
    return name
end

func accept(value: int | string): bool
    if value is int:
        n: int = value as int
        return n == 7
    end

    if value is string:
        s: string = value as string
        return s == "Ivy"
    end

    return false
end
```

## Function Declarations

A Flower function is declared with:

```flo
func name(param: Type): ReturnType
    ...
end
```

Example:

```flo
func multiply(a: int, b: int): int
    return a * b
end
```

The current declaration shape is very explicit:

- the function name is required
- parameters are typed
- return type is required
- the function body is a statement list
- the function closes with `end`

Flower does not currently use inference for parameter or return types in function declarations. You say what the function takes, and you say what it returns.

### Function Exports

```flo
prop func square(x: int): int
    return x * x
end
```

A plain `func` is module-private by current compiler enforcement / rules.

That means a common Flower pattern is:

```flo
func helper(x: int): int
    return x + 1
end

prop func use_helper(x: int): int
    return helper(x)
end
```

The private helper stays internal to the file. The exported function becomes the public surface, essentially accessible API.

### Top-Level Only

Functions are currently a top-level declaration form.

That means Flower currently supports function declarations at module scope, not nested inside other function or blocks.

## Parameters

Function parameters are written as:

```flo
name: Type
```

and separated by commas.

Example:

```flo
func add(a: int, b: int): int
    return a + b
end
```

A zero-argument function is written normally:

```flo
func main(): int
    return 0
end
```

Parameters currently use the same general type surface as the rest of Flower. That includes:

- primitive types
- pointers
- arrays
- transparent aliases
- nullable types
- semantic unions
- structs and raw storage unions

Example:

```flo
type Count = int

func identity(x: Count): Count
    return x
end

func pass_name(name: ?string): ?string
    return name
end

func accept(value: int | string): bool
    return true
end
```

### Parameter Rules

Current compiler rules:

- argument count must match parameter count
- argument types must match parameter types under current type compatibility rules
- aliases are transparent
- explicit casts are still required where Flower would not otherwise assume conversion
- semantic union parameters are allowed
- nullable parameters are allowed
- you cannot declare a struct, union, or alias inside a parameter

If a call provides too many or too few arguments, the compiler reports a call / parameter count mismatch.

If an argument does not match the expected type, the compiler reports a function argument type error.

## Return Types

Function return types are explicit.

Example:

```flo
func add(a: int, b: int): int
    return a + b
end
```

Flower currently supports return types including:

- primitives
- pointers
- arrays
- aliases
- `void`
- nullable types
- semantic unions
- structs and raw storage unions

Do note that just like parameters, return types can not currently declare inline structs, unions, or aliases.

Example:

```flo
type RemoteValue = int | string

func make_int(): RemoteValue
    return 11
end

func make_string(): RemoteValue
    return "Rose"
end
```

### `void`

Use `void` when a function does not return a value:

```flo
func log_name(name: string): void
    print(name)
end
```

A `void` function may also use a bare `return` to exit the function early:

```flo
func maybe_log(name: string): void
    if name == "":
        return
    end

    print(name)
end
```

### Return Rules

Current compiler rules:

- a `return value` must satisfy the function's declared return type under current type-compatibility rules.
- a bare `return` only matches a `void` return
- union and nullable return types are allowed
- string, alias, nullable, and union rules still apply to returned expressions

Example:

```flo
func pass_count(value: ?int): ?int
    return value
end
```

Example with a semantic union:

```flo
type Value = int | string

func make_value(): Value
    return "Ivy"
end
```

One important detail: Flower checks the type of explicit `return` statements, but there is not a full polished "all control paths must return" system in place, as that's out of scope for the current compiler's features.

## `return`

`return` exits a function.

It can be written in two forms:

### Returning a Value

```flo
return x + y
```

### Bare Return

```flo
return
```

Bare `return` is for no-value returns, which in practice means `void` functions.

Example:

```flo
func early_exit(flag: bool): void
    if flag:
        return
    end

    print("continued\n")
end
```

## Function Calls

A direct function call looks like:

```flo
add(1, 2)
```

A call is an expression in Flower. That means it can appear in places like:

- variable initializers
- assignments
- comparisons
- return statements
- arguments to other calls

Examples:

```flo
x: int = add(1, 2)

if add(1, 2) == 3:
    print("ok\n")
end

return multiply(3, 4)
```

Calls can also appear as standalone statements when you only want their side effects:

```flo
log_name("Ivy")
math.init()
```

### Imported Alias Calls

Flower also supports namespaced calls through imported module aliases:

```flo
import "examples/import/math.flo" as math

func main(): int
    value: int = math.add(1, 2)
    return 0
end
```

This is module alias syntax, not method syntax. That distinction matters.

```flo
math.add(1, 2)
```

means:

- `math` is an imported module alias
- `add` is an exported function in that module

It does **not** mean:

- `math` is an object instance with methods

Flower keeps those ideas separate on purpose.

### Common Calling Patterns

Direct helper call:

```flo
func add(a: int, b: int): int
    return a + b
end

func main(): int
    x: int = add(1, 2)
    return x
end
```

Imported namespace call:

```flo
import "examples/import/math.flo" as math

prop func test(): bool
    return math.add(1, 2) == 3
end
```

Union-aware function result after storing:

```flo
value: int | string = make_string()

if value is string:
    s: string = value as string
    print(s)
end
```

### Built-in Statement-Like Forms

Some Flower forms look call-like in source but are parsed as dedicated syntax rather than ordinary function-call expressions.

- `print(...)` is currently a dedicated statement form, not a normal `AST_FUNC_CALL`.

## Forward Declarations

Flower currently supports forward function declarations:

```flo
forward func add(a: int, b: int): int
```

A forward function gives the compiler the function's name, parameter list, and return type before the full definition is encountered.

That is useful when source order would otherwise get in the way. Current forward declarations are declaration-only.

They are not function bodies, and they do not use `end`.

Example shape:

```flo
forward func square(x: int): int

func use_square(): int
    return square(4)
end

prop func square(x: int): int
    return x * x
end
```

## Functions and Types

Functions participate in the same type system as the rest of Flower.

That means function surfaces naturally interact with:

- transparent aliases
- `string`
- `bool`
- nullable values
- semantic unions
- explicit extraction via `as`
- narrowing via `is` after storing or otherwise stabilizing a value

### Alias Example

```flo
type Count = int
type Total = int

func identity(x: Count): Total
    return x
end
```

Aliases improve meaning, but they do not create a separate nominal function type world.

### Nullable Example

```flo
func pass_name(name: ?string): ?string
    return name
end

prop func test(): bool
    value: ?string = pass_name("Ivy")
    if value != null:
        real: string = value as string
        return real == "Ivy"
    end

    return false
end
```

### Semantic Union Example

```flo
type RemoteValue = int | string

prop func make_string(): RemoteValue
    return "Rose"
end

prop func test(): bool
    value: RemoteValue = make_string()

    if value is string:
        real: string = value as string
        return real == "Rose"
    end

    return false
end
```

The important current rule is that a union-returning function is fine, but Flower still expects explicit narrowing and extraction before member-specific use.

## Common Function Shapes

### Small Arithmetic Helpers

```flo
func add(a: int, b: int): int
    return a + b
end

func multiply(a: int, b: int): int
    return a * b
end
```

### Public Test-Style Entrypoint

```flo
prop func test(): bool
    return true
end
```

### Program Entrypoint

Flower programs commonly expose `main`:

```flo
func main(): int
    return 0
end
```

In lower-level programs, especially the compiler itself, you may also need a more explicit C-shaped entry:

```flo
func main(argc: int, argv: @char[]): int
    return 0
end
```

This page is not claiming any runtime signature for all future Flower targets. This is just the current source shapes that exist in the project as of right now.

## Rules

Current function rules worth keeping in the back of your mind:

- functions use explicit parameter and return types
- `prop func` exports a function
- plain `func` is module-private
- direct calls use `name(...)`
- imported module calls use `alias.name(...)`
- parameters are typed and comma-separated
- return values must satisfy declared return types under current type-compatibility rules
- bare `return` matches `void`
- aliases stay transparent across function boundaries
- nullable and semantic union function types are supported
- semantic union results still require explicit narrowing and extraction before member-specific use
- function calls are treated as expressions
- forward declarations exist for functions

If Flower seems strict here, that's because it is meant to be. Functions are one of the places where assumptions get expensive fast, so the language prefers to keep the contract specific in the source.

## Examples

### Private helper plus exported function

```flo
func add(a: int, b: int): int
    return a + b
end

func multiply(a: int, b: int): int
    return a * b
end

prop func test(): bool
    x: int = add(1, 2)
    if x == 3:
        y: int = multiply(3, 4)
        if y == 12:
            return true
        end
    end

    return false
end
```

### Imported module alias call

```flo
import "examples/import/math.flo" as math

prop func test(): bool
    return math.add(1, 2) == 3
end
```

### Nullable function parameter and return

```flo
func pass_name(name: ?string): ?string
    return name
end

prop func test(): bool
    value: ?string = pass_name("Ivy")
    if value != null:
        text: string = value as string
        return text == "Ivy"
    end

    return false
end
```

### Semantic union return

```flo
type RemoteValue = int | string

prop func make_int(): RemoteValue
    return 11
end

prop func make_string(): RemoteValue
    return "Rose"
end

prop func test(): bool
    value: RemoteValue = make_string()

    if value is string:
        s: string = value as string
        return s == "Rose"
    end

    return false
end
```

### Forward declaration

```flo
forward func square(x: int): int

func use_square(): int
    return square(4)
end

prop func square(x: int): int
    return x * x
end
```

## Notes From The Field

Functions are one of the clearest examples of Flower's personality.

The language does not want function boundaries to feel hidden or 'magical.' It wants them to feel explicit, legible, and even stubborn (in a good way)! You say what goes in. You say what comes out. If something may be absent, or may be one of several things, that should still be visible at the function boundary rather than smuggled through as an unspoken convention.

That makes simple helper functions feel very calm. It also makes more advanced cases like nullable and semantic union returns feel direct instead of slippery. The source has to admit what is happening.

Flower also keeps module alias calls separate from object-oriented method ideas. `math.add(...)` is a namespaced module call, not a method on an instance. That separation is part of the language staying procedural and compiler-owned rather than drifting into accidental OOP habits.

## What Is Not Here Yet

Some important function-related things are intentionally missing or still limited:

- nested function declarations
- anonymous functions / lambdas
- first-class function values
- function pointer types
- default arguments
- named arguments
- variadic user-defined functions
- function overloading
- method / receiver syntax
- implicit extraction after narrowing
- narrowing directly on unstable call results
- polished all-path missing-return analysis as a documented language guarantee

## Why It Is Not Here Yet

These gaps exist for different reasons.

### Nested functions, lambdas, and first-class function values

Those are a much larger design commitment than Flower is currently ready for. They would drag in new questions about capture, scope, lifetime, representation, and function types. Flower is not there yet, and I won't pretend otherwise.

### Default arguments, named arguments, and variadics

Those are convenience features, but they are still real semantics. Right now Flower is prioritizing a clear and explicit core function model over call-site sugar.

### Overloading and method syntax

Functions are currently resolved via direct name or module alias plus name. That keeps the system simple and explicit. Overloading and receiver-style method syntax would push the language toward a different identity and a more complex name-resolution model.

### Narrowing directly on call results

This is one of those things that may seem small until you actually try to pin it down. The problem is expression stability. A stored local value is easy to reason about. A call result is not automatically the same kind of stable semantic thing. Flower currently keeps that difference explicit until it is ready to approach the subject.

### Missing-return analysis

The compiler is already serious about type-checking explicit `return` statements. A stronger "all paths must return" feature is a separate layer of control-flow reasoning and should be documented only when it is truly enforced as part of the language surface.

## Related Docs

- [`docs/STRUCTURE.md`](../STRUCTURE.md)
- [`docs/DOCUMENTATION.md`](../DOCUMENTATION.md)
- [`docs/ROADMAP.md`](../ROADMAP.md)
- [`docs/language/types.md`](../language/types.md)
- [`docs/Milestones/v1.4.0-better-types.md`](../Milestones/v1.4.0-better-types.md)
