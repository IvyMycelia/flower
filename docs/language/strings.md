# Strings

`string` is the built-in text type. `@char` represents pointer-based character data. Conversion between `string` and `@char` is explicit.

This page documents:

- the `string` type
- string literals
- equality and inequality
- `.length`
- indexing
- printing
- conversion to and from `@char`
- standard-library string helpers
- relevant interoperability behavior
- current limitations

## Syntax

A string value is declared with the `string` type:

```flo
person: string
title: string = "Flower"
empty: string = ""
```

String literals use double quotes:

```flo
"Hopper"
"Flower"
""
"Hello\n"
```

A raw character pointer uses `@char`:

```flo
raw: @char
```

## String Literals

A string literal can be used when either `string` or `@char` is expected.

Examples:

```flo
color: string = "Yellow"
raw: @char = "Pizza"
```

The literal syntax is the same in both cases. The surrounding type determines how the literal is lowered.

This also applies in other typed contexts, such as parameters, returns, struct fields, and semantic unions.

```flo
type Value = int | string

value: Value = "world!"
```

## Behavior

### Equality and Inequality

String supports `==` and `!=`.

```flo
if food == "taco":
    print("match\n")
end

if food != other:
    print("different\n")
end
```

String ordering comparisons are not supported.

```flo
// Invalid: ordering comparisons are not supported for strings.
if name < other:
    print("bad\n")
end
```

### Length

Strings supports `.length`.

```flo
letters: string = "abcd"
count: int = letters.length
```

`.length` returns an `int`.

It is the only property on `string` that is accessible.

```flo
// Invalid: unknown string field.
name.data = "new data"
```

`.length` is also read-only.

```flo
// Invalid: string length is read-only.
name.length = 99
```

### Indexing

Strings support indexing with `[]`.

```flo
values: string = "TAGC"

first: char = name[0]
second: char = name[1]
third: char = name[2]
```

String indexing returns `char`, and is read-only.

```flo
// Invalid: string indexing is read-only.
name[1] = 'U'
```

### Printing

`print(...)` accepts strings directly.

```flo
username: string = "Bob"
print("Welcome, ")
print(username)
```

## Conversions

Explicit conversion between `string` and `@char` is supported.

### `string` to `@char`

If you need raw character pointer data, then use `as @char`.

```flo
topping: string = "mushroom"
raw: string = topping as @char
```

### `@char` to `string`

If you need a proper Flower string instead, use `as string`.

```flo
raw: @char = "Hello, World!"
text: string = raw as string
```

### Round Trip

```flo
code: string = "Abcd1234"
raw: @char = code as @char
other: string = raw as string

if code == other:
    print("same\n")
end
```

## Type Interactions

### Aliases

Type aliases can name string-based types.

```flo
type Name = string
type MaybeName = ?string
type Value = int | string
```

Aliases do not create a distinct nominal string type.

### Nullable Strings

Strings can participate in nullable types.

```flo
maybe_name: ?string = null
maybe_name = "bob"

if maybe_name != null:
    text: string = maybe_name as string
    print(text)
end
```

### Semantic Unions

Strings can participate in semantic unions.

```flo
type Value = int | string

value: Value = "Rose"

if value is string:
    text: string = value as string
    print(text)
end
```

### Struct Fields

Struct fields and semantic-union fields can contain string types.

```flo
struct User {
    name: string
}

type Value = int | string

struct Box {
    value: Value
}
```

## Standard Library

The standard-library helper functions for strings are located in:

```text
src/stdlib/string.flo
```

The library is not at all complete, and exists purely for some convenience within the compiler itself as well as a proof-of-concept.

The helpers that currently exist are:

- `strings.is_empty(s: string): bool`
- `strings.starts_with(s: string, prefix: string): bool`
- `strings.ends_with(s: string, suffix: string): bool`
- `strings.find_char(s: string, ch: char): int`

Example:

```flo
import "src/stdlib/string.flo" as strings

name: string = "Bob"

if strings.starts_with(name, "Bo"):
    print("prefix\n")
end

if strings.ends_with(name, "ob"):
    print("suffix\n")
end

index: int = strings.find_char(name, 'o')
```

## Interoperability

### Raw Character Interfaces

Backend-facing interfaces generally expect `@char`, not `string`.

Thus, you should use an explicit conversion when passing a string value to such an interface.

```flo
greeting: string = "Hello!"
printf("%s\n", greeting as @char)
```

### String Literals in Recognized Backend Calls

Under the current C backend, string literals are lowed as raw character-pointer arguments in certain builtin call positions, including cases such as:

- `printf`
- `fprintf`
- `snprintf`
- `fopen`
- `system`
- `getenv`
- `realpath`

Example:

```flo
printf("Hello, %s\n", "world")
mode: @char = "r"
file: @char = fopen("data.txt", mode)
```

This is specific to these backend calls, not a general rule that all string values automatically satisfy raw character-pointer interfaces. It exists primarily as a workaround to a lacking standard library.

### Backend-Specific Representation

Under the current C backend, `string` lowers to a runtime struct carrying character data and length.

That representation is an implementation detail, and is relevant mainly because:

- `print(...)` can print string values directly
- backend C functions usually require `@char`
- conversion from `@char` to `string` assumes pointer-based character data appropriate for the current backend

In practice, conversion from `@char` to `string` follows typical C-string conventions.

## Limitations

The following are not currently supported string features:

- concatenation operators
- ordering comparisons such as `<` or `>`
- mutable string indexing
- substring or slice operations
- interpolation
- method syntax such as `name.starts_with("Bo")`
- a backend-independent string runtime model

String helpers beyond the current standard-library functions are also still limited.

## Related

- [Types](../language/types.md)
- [Functions](../language/functions.md)
- [Memory](../language/memory.md)
- [Structs and Unions](../language/structs-unions.md)
- [Project Structure](../STRUCTURE.md)
- [Roadmap](../ROADMAP.md)
- [v1.4.0 Better Types](../Milestones/v1.4.0-better-types.md)
