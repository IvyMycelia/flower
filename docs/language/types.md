# Types

## What This Covers

This page describes Flower's current type surface.

That includes:

- primitive types
- pointers
- arrays
- `string`
- `bool`
- transparent aliases
- `null`
- nullable sugar via `?T`
- semantic unions via `A | B`
- narrowing with `is`
- explicit extraction and conversion with `as`

This page is meant to describe how types work *today*, not how they might behave later if Flower grows a new type system.

## Current Surface

Flower currently has these major type forms:

### Primitives

```flo
int
float
double
char
bool
string
void
```

### Pointers

```flo
@int
@char
@Node
```

### Arrays

```flo
int[]
char[]
string[]
```

### Transparent Aliases

```flo
type Size = int
type Name = string
type NodeRef = @Node
type MaybeName = ?string
type Value = int | string
```

### Nullables

```flo
?int
?string
?@Node
```

### Semantic Unions

```flo
int | string
int | float | double
string | null
```

### Raw Storage Unions

```flo
union Bits {
    i: int,
    f: float
}
```

Raw storage `union` and semantic `A | B` are not the same thing. That distinction matters, and Flower keeps it on purpose.

## Primitive Types

Flower's primitive types are the base materials the rest of the type system builds on.

### `int`

`int` is the default whole-number type.

```flo
count: int = 3
delta: int = -1
```

Use `int` when you want ordinary integer math, counters, indexes, and the general "this is a number with no fractional part" case.

### `float`

`float` is the smaller floating-point type.

```flo
ratio: float = 0.5
```

Use it when you want a decimal number and do not need the wider `double` form.

### `double`

`double` is the wider floating-point type.

```flo
pi: double = 3.1415926535
```

If the distinction matters to you, `double` is the more explicit "I want the wider decimal type" choice.

### `char`

`char` is for a single character-sized value.

```flo
letter: char = 'A'
newLine: char = '\n'
```

It is not the same thing as `string`, and Flower keeps that distinction very deliberately.

### `bool`

`bool` is a real semantic type in Flower.

```flo
ready: bool = true
failed: bool = false
```

In Flower, `bool` is not treated as a wrapper for `0` or `1`. In the current C backend it lowers to integer shaped output, but at the Flower language level it is its own type with its own rules.

Practical consequences:

- conditionals expect `bool`
- arithmetic operators do not accept `bool` operands as if they were ordinary integers
- `true` and `false` are real boolean literals

### `string`

`string` is a real Flower type.

```flo
name: string = "Ivy"
```

It is not merely `@char` renamed. In current Flower semantics, `string` carried a specific meaning:

- equality / inequality
- `.length`
- indexing
- explicit casts to / from `@char`

Example:

```flo
name: string = "Ivy"

if name == "Ivy":
    print(name)
end

print(name.length)
print(name[0])
```

Current practical rule:

- use `string` when you mean text as a Flower value
- use `@char` when you mean raw character-pointer-style data

### `void`

`void` is the no-value type.

It is primarily used for function returns:

```flo
func log_name(name: string): void
    print(name)
end
```

It is not an ordinary value-carrying type.

### Pointer Types

Pointer types are written with `@`.

```flo
@int
@char
@Node
```

A pointer type means "a value that refers to a `T` somewhere else."

Examples:

```flo
node: @Node
buffer: @char
value_ptr: @int
```

Pointers matter in Flower because the language is still low-level:

- explicit indirection exists
- manual memory exists
- raw storage concerns still matter
- Flower does not hide that behind fluff

Typical pointer-related operations are:

- declare a pointer type with `@T`
- take an address with `&x`
- dereference with `@x`
- allocate with `new`
- free with `prune`

Example:

```flo
struct Node {
    value: int
}

func main(): int
    node: @Node = new Node
    node.value = 3
    prune node
    return 0
end
```

One important Flower detail:

Flower source uses `.` for field access. The compiler decides how that lowers later, so there's no need to write C-style `->` in Flower source.

### Array Types

Array types are written with `[]`.

```flo
int[]
char[]
string[]
```

An array type means "a sequence of values whose elements all have the same type."

Example:

```flo
values: int[]
letters: char[]
names: string[]
```

Arrays and pointers are not the same thing, even if the backend may sometimes lower them in similar ways.

Arrays are about:

- element type
- indexing
- ordered grouped data

Pointers are about:

- indirection
- addresses
- explicit memory / reference behavior

If you are choosing between them, the question is usually:

- "is this a sequence of typed elements?" -> array
- "is this a reference / raw memory-oriented value?" -> pointer

### Aliases

Aliases are declared with `type`:

```flo
type Size = int
type Name = string
type NodeRef = @Node
type MaybeName = ?string
type Value = int | string
```

Aliases in Flower are **transparent**.

That means:

- they improve meaning and readability
- they do not create a branch-new nominal runtime identity
- the compiler resolves them as part of semantic checking

Example:

```flo
type Size = int

width: Size = 80
height: Size = 25
```

`Size` here is useful because it says what the value *means*, not because it creates a separate kind of integer that can never be confused with another one.

This choice was made intentionally: aliases help name intention, but they do not invent extra abstraction layers unless the language explicitly decides to later.

### `null`

`null` is a real built-in Flower value.

```flo
null
```

It is not just a backend accident or a C habit that leaked through the floorboards.

`null` exists to express absence.

Examples:

```flo
next: ?@Node = null
name: ?string = null
maybe_count: ?int = null
```

The important thing is that `null` does **not** mean "everything is nullable by default."

Flower keeps the absence of a value explicit.

If a value may be absent, its type should say so.

### Nullable Types

Nullable types are written with `?T`:

```flo
?int
?string
?@Node
```

Current Flower meaning:

```flo
?T == T | null
```

This is a semantic model.

Examples:

```flo
maybe_count: ?int = null
maybe_name: ?string = null
next: ?@Node = null
```

Nullable is not a separate magical category floating above the rest of the system. It is Flower's way of saying:

- this value can be a `T`
- or it can be `null`

Example:

```flo
name: ?string = "Ivy"

if name != null:
    real_nameL string = name as string
    print(real_name)
end
```

That style may feel a little stricter than some languages, but it is intentional. Flower would rather you say exactly when you are crossing from "maybe" to "definitely." This is the current model, and it is unlikely to change — though not impossible.

## Semantic Union Types

Semantic unions are written with `|`:

```flo
int | string
int | float | double
string | null
```

A semantic union means:

> this value may be one of several valid types, and the compiler should track that as part of the language semantics.

Example:

```flo
value: int | string = 7
value = "Ivy"
```

### Semantic Union `A | B`

This is different from raw storage `union`, as it's more about meaning.

It answers:

- what values are valid here
- what the compiler should enforce
- what must be narrowed before use

### Raw Storage `union`

Storage unions can be declared using `union`:

```flo
union Bits {
    i: int,
    f: float
}
```

This is about layout. It answers:

- how memory overlaps
- how raw representation is shared
- how low-level storage is controlled

Flower keeps raw storage unions and semantic unions separate because they solve different problems. If they were collapsed into one thing, the language would get blurrier, not simpler (believe me, I tried).

## Narrowing with `is`

Use `is` when you want to ask whether a value or semantic union currently holds a specific member type.

Example:

```flo
value: int | string = 7

if value is int:
    n: int = value as int
    if n is int:
        print(n)
    end
end
```

This is called narrowing because the compiler moves from:

- "this could be several things"

to:

- "inside this branch, I know which thing it is"

Current Flower support includes narrowing on:

- local union values
- parameters
- return values after user
- struct fields
- stable field-expression chains

Example:

```flo
struct Box {
    value: int | string
}

box: @Box = new Box
box.value = "Ivy"

if box.value is string:
    label: string = box.value as string
    print(label)
end
```

For nullable values, the common narrowing form is a null check:

```flo
next: ?@Node = null

if next != null:
    real_next: @Node = next as @Node
end
```

## Explicit Extraction and Conversion with `as`

`as` is Flower's explicit conversion and extraction operator.

It is used in a few important situations.

### 1. Extracting after narrowing

```flo
value: int | string = 7

if value is int:
    n: int = value as int
end
```

### 2. Extracting after null checks

```flo
name: ?string = "Ivy"

if name != null:
    real_name: string = name as string
end
```

### 3. Converting between types that should not be assumed

```flo
raw: @char = "Ivy" as @char
text: string = raw as string
```

### 4. Numeric conversions when you want to be explicit

```flo
ratio: double = 4 as double
count: int = ratio as int
```

### 5. Converting booleans and integers into one another

```flo
func is_checked(): int
    return true as int
end
```

Current practical rule:

If the compiler cannot obviously and safely treat two types as the same thing — or convertable between one another — write `as`.

That is not ceremony for its own sake, but rather it is Flower forcing the moment of intent into the source.

## Rules

Current type-system rules worth keeping in the back of your head:

- declared types matter
- aliases are transparent
- `bool` is distinct from ordinary integer semantics
- `string` is distinct from raw `@char`
- `null` is real, but not universal
- `?T` means `T | null`
- semantic unions must be narrowed before member-specific use
- raw storage `union` and semantic union `A | B` are not the same
- Flower prefers explicit extraction and conversion over broad coercion

If you are ever unsure what Flower wants from you, the safest bet is usually:

- name the type explicitly
- narrow explicitly
- cast explicitly

That is usually where the language is trying to push you anyway.

## Examples

### Primitive Values

```flo
count: int = 10
ratio: float = 0.5
pi: double = 3.1415926535
letter: char = 'A'
ready: bool = true
name: string = "Ivy"
```

### Aliases

```flo
type Size = int
type Label = string

width: size = 80
title: Label = "Flower"
```

### Nullable Values

```flo
maybe_name: ?string = null
maybe_count: ?int = 4

if maybe_name != null:
    value: string = maybe_name as string
    print(value)
end
```

### Semantic Unions

```flo
type Value = int | string

value: Value = 7

if value is int:
    n: int = value as int
    print(n)
end

value = "Ivy"

if value is string:
    s: string = value as string
    print(s)
end
```

### Stable Field-Expression Narrowing

```flo
struct Box {
    value: int | string
}

box: @Box = new Box
box.value = "Ivy"

if box.value is string:
    name: string = box.value as string
    print(name)
end

prune box
```

## Notes From The Field

Flower's type system is trying to do something *very* specific.

It does not want to become "C with prettier spelling," but it also does not want to become a foggy abstraction machine that hides every sharp edge until you step on it barefoot.

So the type system tends to divide the world cleanly:

- semantic meaning stays semantic
- low-level storage stays low-level
- optionality is explicit
- narrowing is explicit
- extraction is explicit

That can feel super strict, especially compared to some other languages. But it can also feel calmer ocne you get used to it, because the source tends to say when it is crossing from one state to another. Types in Flower are not decorative labels. They are the compiler's way of keeping track of what kind of thing you are actually carrying, what it can become, and what you still have to prove before using it as something narrower.

## What Is Not Here Yet

Some important things are intentionally missing or still limited:

- implicit extraction after narrowing
- narrowing on call results
- exhaustiveness checking for semantic unions
- pattern matching
- nominal newtypes distinct from semantic unions
- a dedicated deep-dive on raw storage `union` layout and behavior in this page

## Why It Is Not Here Yet

These gaps exist for different reasons.

### Implicit post-narrow extraction

Flower currently requires explicit `as` after narrowing because the language is still leaning toward visible transitions rather than assuming what you mean at every step. This is unlikely to change.

### Narrowing on call results and subscript expressions

These are juch more complicated because they raise questions about stability, repeat evaluation, and what exactly counts as the same expression during semantic analysis. That is not to say it won't be done, but it won't be implemented anytime soon.

### Exhaustiveness checking and pattern matching

Those are way bigger features than they look. They want a more mature union first, rather than arriving early and forcing the rest of the type system to shape itself. I'm worried that if I try to focus on them too much, it means the rest won't get to develop properly and Flower will end up with a messy type system.

### Nominal newtypes

Transparent aliases solve naming and readability now. A real nominal newtype feature would be a different design commitment entirely and should not be smuggled in by pretending aliases are already that. Their time will come. When? I don't know, but it will!

## Related Docs

- [`docs/STRUCTURE.md`](docs/STRUCTURE.md)
- [`docs/DOCUMENTATION.md`](docs/DOCUMENTATION.md)
- [`docs/ROADMAP.md`](docs/ROADMAP.md)
- [`docs/Milestones/v1.4.0-better-types.md`](docs/Milestones/v1.4.0-better0types.md)
