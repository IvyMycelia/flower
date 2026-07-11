# Control Flow

## What This Covers

This page documents Flower's current control-flow surface: branching, loops, range-based iteration, and statements that let execution skip, leave, or return.

Control flow in Flower is intentionally small — like most other features — right now. There is no jungle of special forms. The current language provides a handful of explicit tools and expects you to build readable logic out of them, however it is not strictly enforced.

What currently exists is:

- `if`, `else if`, `else`
- `while`
- `for`
- `return`
- `break`
- `continue`

It also covers how conditions work, because in Flower a condition is more than just syntax as it affects typing, narrowing, and sometimes what the compiler can prove inside of a branch.

## Current Surface

Flower currently supports these control-flow forms:

```flo
if condition:
    ...
end

if condition:
    ...
else:
    ...
end

if first:
    ...
else if second:
    ...
else:
    ...
end

while condition:
    ...
end

for i in 0..10:
    ...
end

for i in 0..=10:
    ...
end

return value
break
continue
```

Blocks are started with `:` and closed with `end`. Flower does not use braces for blocks.

## Rules

### Conditions

A condition must currently be a condition-compatible value.

As of right now, that means the compiler accepts:

- `bool`
- integer-like values
- pointer-like values

So this currently works:

```flo
flag: bool = true
count: int = 3
ptr: @int = null

if flag:
    ...
end

if count:
    ...
end

if ptr:
    ...
end
```

That is the current compiler surface, even if long-term language direction changes to become stricter later on.

## Logical Conditions

Flower supports `and`, `or`, and `not` in conditions.

```flo
if a and not b:
    ...
end
```

Both sides of `and` / `or` must be condition-compatible. The operand of `not` must also be condition-compatible.

## `if`, `else if`, and `else`

`if` branches are the main way to express conditional logic.

```flo
if x > 0:
    print("positive")
else if x < 0:
    print("negative")
else:
    print("zero")
end
```

`else if` is written literally as `else if`. It is not a separate keyword. It is an `else` whose body is another `if`.

### `while`

`while` repeats for as long as its condition remains truthy under the current condition rules.

```flo
i: int = 0

while i < 5:
    print(i)
    i = i + 1
end
```

### `for`

Flower currently supports range-based `for` loops.

```flo
for i in 0..5:
    print(i)
end
```

`..` is exclusive on the upper bound.

That means the example above iterates:

```flo
0, 1, 2, 3, 4
```

Flower also supports inclusive ranges through `..=`:

```flo
for i in 0..=5:
    print(i)
end
```

That iterates:

```flo
0, 1, 2, 3, 4, 5
```

Right now, this is the `for` loop surface. It is a range loop, not a general iterator loop. You cannot currently use for loops to directly iterate over things such as arrays. Loop range variables are always lowered to `int` in order for them to work, and lowering supports both ascending and descending ranges.

### `return`

`return` exits the current function.

```flo
prop func abs(x: int): int
    if x < 0:
        return -x
    end

    return x
end
```

A return value must match the function's return type under the compiler's current typing rules.

The only "exemption" to a return requiring a value is with void functions:

```flo
prop func print_name(s: string): void
    if s.length == 0:
        return
    end

    print(s)
end
```

### `break`

`break` exits the current loop.

```flo
i: int = 0

while i < 10:
    if i == 5:
        break
    end

    i = i + 1
end
```

Note that Flower does not currently enforce loop-only placement here, so a `break` outside of a loop is not diagnosed cleanly by Flower and may fail later during C compilation.

### `continue`

`continue` skips the rest of the current loop iteration and moves on to the next one.

```flo
for i in 0..10:
    if i == 5:
        continue
    end

    print(i)
end
```

Just as with `break`, `continue` also does not have enforced loop-only placement.

## Narrowing Inside Control Flow

Conditions can refine what the compiler knows.

For example, nullable and semantic-union checks can narrow inside a branch:

```flo
if maybe is @int:
    live: @int = maybe as @int
    print(@live)
end
```

The important current rule is that:

Even after narrowing, Flower still prefers explicit `as` at the use site in places where the narrowed value needs to be treated as a more specific type.

The branch proves what the value can be, and the cast states what you are doing with said value.

## Examples

### Branching

```flo
prop func describe(n: int): int
    if n > 0:
        print("positive")
        return 1
    else if n < 0:
        print("negative")
        return -1
    else:
        print("zero")
        return 0
    end
end
```

### While Loop

```flo
prop func countdown(start: int): void
    n: int = start

    while n > 0:
        print(n)
        n = n - 1
    end
end
```

### Exclusive and Inclusive Ranges

```flo
for i in 0..3:
    print(i)
end

for i in 0..=3:
    print(i)
end
```

### Guard Clause Style

```flo
prop func print_name(name: ?string): void
    if not (name is string):
        return
    end

    real_name: string = name as string
    print(real_name)
end
```

## Notes From The Field

Flower's control flow is simple on purpose, this was my intentional decision.

The language is not trying to win by having the most forms. Rather, it is trying to stay readable, explicit, and low-drama while the type system keeps getting sharper.

That is also why some things that other languages may silently "help" with are still spelled out here. Narrowing exists, but explicit casts still matter. Range loops exist, but foreach-style iteration does not. `else if` works, but it is still visibly just `else` plus `if`.

This keeps the language honest. You can usually tell what the compiler is going to believe by looking directly at the code.

## What Is Not Here Yet

The following are not part of the current documented control-flow surface:

- `switch` / `match`
- pattern matching
- foreach-style iteration of arrays, strings, or custom containers
- labeled `break`
- labeled `continue`
- ternary expressions
- exhaustiveness checking for branch-heavy type tests
- implicit cast elision after every successful narrow

## Why It Is Not Here Yet

Most of that is deferred for the same reason: Flower is still stabilizing its core relationship between syntax, narrowing, and lowering. These features require a more robust type system that is currently not present within Flower.

Right now, the compiler already has enough moving parts in:

- branch conditions
- nullable checks
- semantic unions
- explicit casts after narrowing
- loop lowering
- self-hosted verification

Adding richer control-flow forms before that core is fully established would make the language nosier before it became better.

So the current rule is simple in my opinion: keep the control-flow surface compact, make it explicit and honest, and only add bigger forms once the existing ones have stopped shifting under our feet.

## Related Docs

- [Types](../language/types.md)
- [Functions](../language/functions.md)
- [Project Structure](../STRUCTURE.md)
- [Roadmap](../ROADMAP.md)
