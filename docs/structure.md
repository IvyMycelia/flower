# Grammar

> Note: This document is still being rewritten. The sections below reflect current Flower syntax where updated, but some older examples and terminology may still lag behind the compiler. When this document and the compiler disagree, the compiler (obviously) has authority.

## File Structures

### Module Example

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

### Import Example

```rust
/* main.flo */
import "./math.flo" as math

func main(): int
    x: int = math.square(4)

    if x == 16:
        prin("ok\n")
    end

    return 0
end
```

Current module rules:

* `prop` marks exported top-level declarations
* top-level declarations are private by default
* imported aliases act as real namespaces
* module / interface rules are enforced by the compiler, not deferred to generated C

### Example main.flo

```go
/* Default program entry */
func main(args: string[]): int
    if args.length > 0:
        if args[2] == "--p":
            print("P Mode enabled\n")
            return 1
        else if args[2] == "--b":
            // Empty block, stack is still pushed but nothing is inside this block. Waits for end to terminate or an else to attach
        else:
            print("Unrecognized symbol\n") return -1 // Flower is white-space and newline insensitive by default
        end // This end tells the compiler that if y == 0 block is done and that anything that follows is for the original if x block
    else print("Not enough arguments\n") // No end is needed because the else only has one statement. The compiler knows this is the end of the whole block, because there's no : after the else meaning whatever follows HAS to be the last statement. NOT IMPLEMENTED YET

    return 0; // Semicolons optional to end a line — really doesn't do anything for reasons stated above
end
```

## Current Type System Snapshot

```go
type Size = int
type MaybeName = string | null

func show_name(name: ?string): void
    if name != null:
        real: string = name as string
        print(real)
    end
end

func describe(value: int | string): void
    if value is int:
        n: int = value as int
        print(n)
    end
end
```

Current rules:

* `type Name = ExistingType` is a transparent alias
* `?T` is sugar for `T | null`
* `A | B` is a semantic union type
* `union` remains a raw storage / layout feature
* `is` narrows
* `as` extracts explicitly after narrowing

## How If Conditionals Work

`else`/`else if` before seeing `end` —> belongs to the **innermost open** `if`
`end` closes the innermost open `if`, "surfacing" to the parent block
`else` with no `:` —> single-statement, no `end` needed, and **implicitly terminates** the entire chain

The compiler maintains a stack: each `if ... :` pushes, each `end` pops, and an `else`/`else if` hooks onto whatever is on top. An implicit `else`/`else if`/`if` self-enforces: because there's no `:` telling the compiler to push a stack, it automatically registers the following statement is part of the conditional's output and treats anything afterwards as independent of the conditional. There's no way to accidentally leave a dangling block, since the absence of `:` structurally prevents continuation.

```rust
if condition:       // colon = open block, ALWAYS needs end
    statements
end

if condition statement          // no colon = single statement, terminated by newline or ;
else if condition statement     // same
else statement                  // same

if condition:       // colon form can still have else/else if before end
    statements
else if condition:
    statements
end
```

### Legal Example

```rust
if x:
    if y:
        print("both")
    end
end
```

### Illegal Example

```rust
if x:
    // ...
end
else if y: // This is illegal because it comes after the stack is popped and the compiler does not know what `else` is referring to
```

```rust
if x
// Empty
// EoF
```

The above is illegal because the compiler never appends anything to the block nor terminates it. Instead of panicking it recognizes that there's no trailing statement following the conditional, and should return an error at compilation. If the user has a statement afterwards, no matter the intention, it will be attached to the block since the language is white-space insesnitive.
