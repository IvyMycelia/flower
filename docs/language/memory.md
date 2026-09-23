# Memory

## What This Covers

- pointer types
- address-of with `&`
- dereference with `@`
- write-through dereference with `@ptr = value`
- allocation with `new`
- release with `prune`
- pointer arithmetic
- indexing through pointers, arrays, and strings
- `sizeof`
- `null` in memory-oriented code
- where Flower's own memory tools end and backend library calls begin

This page is not going to re-document the entire type system, string model, or module system. It will explicitly cover the parts of Flower that put your hands directly on storage and memory handling.

## Memory in Flower

Memory in Flower is very explicit, and the language's goals shine brightest here.

The language does not try to make pointers feel taboo, magical, or hidden behind a tower of abstractions.

This philosophy gives Flower a very plainspoken memory vocabulary:

- `@T` means "pointer to `T`"
- `&x` means "address of `x`"
- `@ptr` means "follow this pointer"
- `@ptr = value` means "write through this pointer"
- `new T` means "allocate space for a `T`"
- `new T[n]` means "allocate space for `n` `T`s"
- `prune ptr` means "release that memory"
- `value[i]` means "index through this thing"
- `sizeof(T)` means "tell me the size of this type"

Flower is low-level by decision. It trusts you enough to let you touch the wires. It does not, however, promise that the wires are always safe.

If you wish to learn about how memory is planned to look in the future, check out [Related Docs](#related-docs).

## Pointer Types

Pointer types use `@`.

```flo
@int
@char
@Node
```

A pointer type means "this value is an address that leads to a `T`."

Examples:

```flo
count_ptr: @int
name_ptr: @char
node_ptr: @Node
```

Multiple levels of indirection can be spelled directly too:

```flo
@@int
```

A pointer value can be:

- stored in a variable
- passed to a function
- compared with `null`
- used in a condition
- dereferenced
- indexed
- moved by integer offset

Pointers to structs still use ordinary Flower field syntax:

```flo
struct Node {
    value: int
}

node: @Node = new Node
node.value = 3
prune node
```

You do not write C-style `->` in Flower. The compiler decides the access form during semantic analysis, and the codegen lowers it later.

## Taking an Address With `&`

`&` takes the address of a value.

```flo
value: int = 42
ptr: @int = &value
```

After that:

```flo
@ptr
```

refers back to the `value`.

It helps to think of `&` and `@` as mirror moves:

- `&value` goes from value to address
- `@ptr` goes from address back to value

Use `&` when you mean "I want to point at this existing storage."

A simple example:

```flo
func bump(ptr: @int): void
    @ptr = @ptr + 1
end

prop func test(): bool
    value: int = 4
    bump(&value)
    return value == 5
end
```

One important note: the ordinary, documented use of `&` is taking the address of storage you intentionally want to reference. This readable case is what this document will stick to.

## Dereference With `@`

In an expression, `@` means dereference.

```flo
value: int = @ptr
```

That means "read the value stored at the address in `ptr`."

Example:

```flo
ptr: @int = new int
@ptr = 42

x: int = @ptr

prune ptr
```

Dereference is not limited to a bare name. It can also be used on a larger pointer expression:

```flo
y: int = @(arr + 1)
```

When pointer arithmetic and dereference meet, parentheses make the intent much easier to read.

### `@` In Types vs `@` In Expressions

Flower uses the same symbol for two related ideas:

- in a type, `@T` means "pointer to `T`"
- in an expression, `@ptr` means "follow this pointer"

This is deliberate. One spelling describes pointer-ness as a type, the other spelling uses that pointer at runtime.

## Writing Through a Pointer

Flower also supports write-through dereference as a statement:

```flo
@ptr = 42
```

That means "store `42` at the location pointed to by `ptr`."

Example:

```flo
ptr: @int = new int
@ptr = 42

if @ptr == 42:
    prune ptr
    return true
end

prune ptr
return false
```

This is the most direct memory-writing form in the language.

### A Narrow Boundary Worth Calling Out

Dereference-as-expression is more flexible than dereference-as-assignment.

Reading through a computed pointer expression is supported:

```flo
y: int = @(arr + 1)
```

Writing through a pointer is still documented in the simple named-pointer form:

```flo
@ptr = value
```

So `@ptr = value` is treated as the real write-through assignment form, not arbitrary dereferenced expressions on the left-hand side.

## Allocation With `new`

`new` allocates memory.

### Allocate One Value

```flo
ptr: @int = new int
node: @Node = new Node
```

`new int` gives you an `@int`.

`new Node` gives you an `@Node`.

### Allocate Several Elements

`new` also accepts an array count in the type shape:

```flo
buffer: @char = new char[256]
table: @int = new int[count]
```

That still returns a pointer to the element type:

- `new char[256]` gives you `@char`
- `new int[count]` gives you `@int`

This is very blunt and direct on purpose: you say the element type, and if you want a count, you write the count right there.

### `new` Is An Expression

Because `new` is an expression, it can appear in places like:

- variable initializers
- assignments
- arguments to other calls

Examples:

```flo
ptr: @int = new int
buffer = new char[size + 1]
log_buffer(new char[64])
```

### What `new` Means Under The C Backend

Under the C backend, `new` lowers to `malloc(...)`.

That matters for two reasons:

1. It explains why `new T[n]` behaves like raw allocated storage rather than some hidden managed container.

2. It exposes a mismatch that has not been cleaned up yet:

   - the backend allocator may return `null`
   - Flower does not yet force `new` to be typed as a nullable result

If you are thinking like a C programmer here, your suspicion is justified.

## Releasing Memory With `prune`

`prune` frees memory.

```flo
prune ptr
```

It is a statement, not an expression.

Typical shape:

```flo
ptr: @int = new int
@ptr = 42
prune ptr
```

A slightly fuller example:

```flo
prop func test(): bool
    p: @int = new int
    @p = 42

    if @p == 42:
        prune p
        return true
    end

    prune p
    return false
end
```

### What `prune` Does Not Do For You

`prune` releases memory, and it does not:

- clear the variable automatically
- prevent a second `prune`
- stop you from dereferencing the pointer afterward
- track ownership for you
- know whether the pointer is dangling

That is still your responsibility.

## `null` In Memory Code

`null` matters a lot with pointers.

A plain pointer can be set to `null`:

```flo
ptr: @int = null
```

You can compare pointer-like values against `null`:

```flo
if ptr != null:
    print(@ptr)
end
```

And pointer-like values are condition-compatible, so this is also accepted:

```flo
if ptr:
    print(@ptr)
end
```

That said, explicit `!= null` checks usually read more clearly in docs and examples, especially once the codebase gets bigger.

### Plain Pointers vs `?@T`

One subtle Flower detail worth noting is:

plain pointer types can already accept `null`.

That means these two ideas are similar and related, but not the same:

```flo
ptr: @int = null
maybe_ptr: ?@int = null
```

`?@int` is not "the only way to be null." What it does is it tells the reader that absence is meant to be part of the contract.

So:

- `@T` can still be null in practice
- `?@T` makes the possibility visible

## Pointer Arithmetic

Flower supports basic pointer arithmetic.

Examples:

```flo
arr: @int = new int[4]

next: @int = arr + 1
also_next: @int = 1 + arr

prune arr
```

You can also dereference the result:

```flo
y: int = @(arr + 1)
```

The documented arithmetic shape is the simple, old-fashioned one:

- pointer-like value plus integer
- pointer-like value minus integer
- integer plus pointer-like value

When it works, the result stays pointer-shaped.

### Why This Exists

Sometimes indexing is the clearer way to accomplish something:

```flo
value: int = arr[1]
```

But pointer arithmetic is still useful when you want to use offsets, cursors, or raw walks through memory.

### What Will Not Be Promised / Discussed Here

This doc will not bless every imaginable pointer algebra trick into idiom.

In particular, it does not present pointer-to-pointer arithmetic or pointer-minus-pointer arithmetic as part of the documented memory vocabulary.

## Indexing With `[]`

Indexing works on several memory-adjacent things:

- arrays
- pointers
- strings

### Pointer / Array Indexing

```flo
buffer: @char = new char[4]
buffer[0] = 'A'
buffer[1] = 'B'

first: char = buffer[0]
second: char = buffer[1]

prune buffer
```

When you index a pointer or array, you get the element type back.

### String Indexing

Strings can also be indexed:

```flo
name: string = "Ivy"
letter: char = name[0]
```

String indexing gives you a `char`.

But strings are not writable character buffers in Flower, so this is not allowed as a documented pattern:

```flo
name[0] = 'R'
```

String indexing is readonly.

### Union Boundary

If the thing you are indexing is a semantic union, Flower expects you to narrow and extract first.

That is part of the general union rule: member-specific behavior comes after explicit narrowing, not prior.

## `sizeof`

Flower reserves the `sizeof` keyword, and it takes a type.

```flo
n: int = sizeof(int)
```

You can also use it in larger expressions:

```flo
buffer: @char = new char[10 * sizeof(char)]
```

### Important Rule

`sizeof` works on a type, not an expression.

That means you can do the following:

```flo
sizeof(int)
sizeof(char)
sizeof(Node)
```

but not:

```flo
sizeof(value)
```

### Result Type

In Flower, `sizeof(...)` is treated as an `int`.

That is the language-facing result, even if the backend target has its own lower-level size type internally.

### A Backend-Shaped Note

The meaning of `sizeof(T)` still depends on the backend layout.

So the syntax and semantics belong to Flower, but the final byte count comes from the target representation underneath.

## Arrays, Pointers, and Text

These three ideas touch each other constantly, but they are not the same thing.

### Arrays

An array type describes element storage with a count.

```flo
char[32]
int[4]
```

### Pointers

A pointer type describes an address.

```flo
@char
@int
```

### Strings

A `string` is a Flower text value.

```flo
string
```

It is not just `@char` with a fake mustache.

### Where They Meet

They *do* overlap in practice:

- arrays and pointers can both be indexed
- arrays may decay toward pointers at call boundaries when the parameter expects that shape
- strings can be indexed to yield `char`
- strings can be cast to `@char` when you truly want raw character-pointer style access

Example:

```flo
text: string = "Rose"
raw: @char = text as @char
round_trip: string = raw as string
```

When `@char` is being used as text, the important convention is raw zero-terminated character data.

So the rough mental split is:

- `string` = Flower-owned text value
- `@char` = raw character pointer
- `char[n]` = fixed array storage of characters

## Backend Library Calls Beside Flower's Own Memory Tools

Flower's own memory verbs are things like:

- `new`
- `prune`
- `@`
- `&`
- `sizeof`
- indexing

But Flower also lets you call backend C library functions after system imports.

That means memory-related code can sit beside backend calls like:

```flo
import <stdlib>

arr: @int = new int
arr = realloc(arr, 10 * sizeof(int))

if arr != null:
    prune arr
end
```

The important distinction is not whether the code compiles, but what kind of promise you are relying on.

- `new`, `prune`, `@`, `&`, and `sizeof` are part of Flower's own memory vocabulary
- `realloc(...)` is a backend library call that Flower can pass through after a system import

While the backend escape hatch is useful, it should not be relied on for explicit support as it belongs to the backend side rather than Flower's own core memory model.

## Common Patterns

### Heap Value

```flo
ptr: @int = new int
@ptr = 42

value: int = @ptr

prune ptr
```

### Raw Buffer

```flo
buffer: @char = new char[256]
buffer[0] = 'O'
buffer[1] = 'K'
buffer[2] = '\0'

print(buffer)

prune buffer
```

### Pass By Address

```flo
func bump(ptr: @int): void
    @ptr = @ptr + 1
end

prop func test(): bool
    value: int = 7
    bump(&value)
    return value == 8
end
```

### Explicitly Optional Pointer Extract

```flo
func read_value(ptr: ?@int): int
    if ptr is @int:
        real: @int = ptr as @int
        return @real
    end

    return 0
end
```

## Notes From The Field

I've noticed a lot of other languages either romanticize low-level power, or apologize for it. Flower does neither. It simply writes it down: a pointer is a pointer, an address is an address, allocation is allocation, and releasing memory is a verb in source.

That's part of Flower's charm.

It also means Flower is still carrying some unresolved tensions. `new` lowers through a backend allocator but is not yet modeled as nullable. Plain pointers can hold `null`, while `?@T` exists to make that absence explicit in the type system. Strings can be indexed like memory, but they are not just raw `@char` buffers.

## What Is Not Here Yet

Some memory-related things are still missing, unfinished, or intentionally pushed off:

- ownership tracking
- borrow checking
- automatic destruction / RAII
- `defer`-style cleanup
- slices or views as their own memory type
- documented write-through dereference of arbitrary pointer expressions
- subtracting one pointer from another as part of the documented toolbox
- bounds-checking guarantees
- allocator semantics reflected faithfully in the type of `new`
- automatic dangling-pointer protection
- a backend-independent runtime memory model

## Why It Is Not Here Yet

### Ownership and automatic cleanup

Those features are not "small ergonomics," and incorporate important decisions about lifetime, aliasing, and responsibility. Flower is not there yet.

### `new` and allocation failure

This is an obvious unresolved spot in Flower. The backend allocator can fail, and the type system with `new` has not fully caught up to that fact yet.

### Backend independence

Right now, some practical memory behavior is still shaped by the C backend: `malloc`, `free`, `realloc`, `NULL`, and native layout. Flower can grow past that, but it should do so consciously rather than pretending that gap is nonexistent.

## Related Docs

- [Project Structure](../STRUCTURE.md)
- [Roadmap](../ROADMAP.md)
- [Types](../language/types.md)
- [Functions](../language/functions.md)
- [Modules](../language/modules.md)
- [v1.4.0 Better Types](../Milestones/v1.4.0-better-types.md)
