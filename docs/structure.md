# Grammar

> Note: This document describes Flower's current surface syntax and structure. It is meant to reflect the language as it exists now, not older experiments or future plans. If this document and the compiler disagree, the compiler wins.

## Source Files

Flower source files use the `.flo` extension.

A flower project is currently organized around modules:

- one file per module
- imports happen at file scope
- exported top-level declarations use `prop`
- non-`prop` top-level declarations are private to the module

## Module Example

```go
/* math.flo */
PI: double = 3.1415926535

prop func square(x: int): int
    return x * x
end

func helper(x: int): int
    return x + i
end
```

## Imports and Visibility

```go
import "./math.flo" as math

func main(): int
    value: int = math.square(4)
    return 0
end
```

Current rules:

- `import "path"` loads another Flower module
- `import "path" as name` gives the import a real namespace alias
- `import <name>` requests a system / backend import (currently exists for C support)
- `prop` marks exported top-level declarations
- top-level declarations are private by default
- module and interface rules are enforced by the compiler

## Top-Level declarations

Flower currently supports these top-level declaration forms:

```go
count: int = 0

type Size = int

struct Node {
    value: int,
    next: @Node,
}

union Bits {
    i: int,
    f: float,
}

forward func square(x: int): int

func inner(): void
    square(4)
end

prop func square(x: int): int
    return x * x
end
```

## Functions

Functions use explicit parameter and return types.

```go
func add(x: int, y: int): int
    return x + y
end
```

Exported functions are written with prop:

```go
prop func square(x: int): int
    return x * x
end
```

Programs normally provide a `main` function:

```go
func main(): int
    return 0
end
```

## Types at a Glance

Flower currently has:

- primitive types: `int`, `float`, `double`, `char`, `bool`, `string`, `void`
- built-in singleton value: `null`
- pointers: `@T`
- arrays: `T[]`
- transparent aliases: `type Name = ExistingType`
- semantic nullable types: `?T`
- semantic unions: `A | B | C`
- raw storage unions via `union`

### Alias Example

```go
type Size = int

size: Size = 4
```

### Nullable Example

```go
next: ?@Node = null

if next != null:
    real_next: @Node = next as @Node
end
```

### Semantic Union Example

```go
value: int | string = 4

if value is int:
    n: int = value as int
end
```

Current rules:

- `type Name = ExistingType` is transparent, not nominal
- `?T` is sugar for `T | null`
- `A | B` is a semantic union type
- `union` is the raw overlapping storage feature
- `is` narrows
- `as` extracts explicitly after narrowing

## Control Flow and Blocks

Flower uses `:` to open a block and `end` to close it.

### If / Else

```go
if value > 0:
    print("positive\n")
else:
    print("non-positive\n")
end
```

### While

```go
i: int = 0

while i < 10:
    print(i)
    i = i + 1
end
```

### For

```go
for i in 0..10:
    print(i)
end
```

For current documentation purposes, this file only documents the stable block form that uses `:` and `end`.

## Field Access

Flower source uses `.` for field access.

```go
node.value = 3

if box.value is string:
    label: string = box.value as string
end
```

The compiler determines the lowered access form during semantic analysis; the surface language does not expose C-style `->`.

## Memory and Ownership

Flower is manually managed.

```go
node: @Node = new Node
node.value = 3
prune node
```

Current model:

- `new` allocates
- `prune` frees
- there is no garbage collecter

## Current Boundaries

The following are intentionally outside the currently documented surface here:

- implicit extraction after union or nullable narrowing
- narrowing on call results
- narrowing on subscript expressions
- non-C backend behavior details

See `docs/ROADMAP.md` and the milestone documents for planned future work.
