# Flower Programming Language

Minimalistic expressive programming language for systems programming, where control to write simple code that works and does powerful things is the center of decentralization.

## Philosophy

The anti-thesis of modern-day programming languages.

Corpo-slop, governance, abstraction maximilism; all things that modern systems have drifted towards for its own sake. A machine-based ecosystem in a machine-based world, where people are no longer at the center. Flower exists as a rejection to that trajectory.

Uniformity and obscurity are broing. Flower embraces clarity over ceremony, control over opacity, and experimentation over enforced staleness.

- Write less, mean just as much (and more!)
- Explicity over implicity, what you say goes
- One way to do it (usually)
- Born from the ashes of conformity and standardization (C)

## What Flower Is

A compiled, self-hosting language that currently targets to C (ignore the irony in it being "born from the ashes"). You write Flower, FloC generates C (for now..), Clang builds it, you get a native binary. No runtimes, no VMs, and no magic other than the feeling of euphoria it gives off.

**Self-Hosting**: The Flower Compiler (FloC) is written in Flower, though the original FloC is written in C (can be accessed under `vendor/flower_c_reference/`).

**Minimal**: 30 Keywords (Less than it's prede~~opre~~scer!); Pointers with `@`, memory with `new` / `prune`, that's pretty much it.

**Extensible**: Different branches have different experiments / implementations. See what interests and tickles your fancy, what excels, and what absolutely sucks and deserves to remain abandoned.

## Examples

```lua
import <stdio>

func factorial(n: int): int
    if n <= 1:
        return 1
    end
    return n * factorial(n - 1)
end

func main(): int
    printf("5! = %d\n", factorial(5))
    return 0
end
```

Structs with manual memory:

```lua
struct Node {
    value: int,
    next: @Node
}

func create_list(values: int[], len: int): @Node
    head: @Node = null
    current: @Node = null

    for i in 0..len:
        node: @Node = new Node
        node.value = values[i]
        node.next = null

        if head == null:
            head = node
        else:
            current.next = node
        end
        current = node
    end

    return head
end
```

## Building

You need a working Flower binary first. The repo keeps a bootstrap C snapshot in `bin/Flower.c`:

Compile [`bin/Flower.c`](https://github.com/IvyMycelia/flower/blob/main/bin/Flower.c) or download the latest [`Flower`](https://github.com/IvyMycelia/flower/releases/tag/Stable) release first to get the executable.

```bash
make build      # Compiles from .flo source and creates new Flower binary under `bin/` if there's no errors
make bootstrap  # Build with verification to ensure the new executable is valid before overwriting the stable, working one
make test       # Run the test suite with `/bin/Flower`
```

## Platform Support

Flower currently builds and bootstraps in Unix-like environments.

Current status:

- **macOSX**: supported and actively used during development
- **Linux**: expected to work with a POSIX shell and a C99-capable compiler, though platform coverage is still limited
- **Windows**: not natively supported yet

The main blocker for native Windows support is not the Flower language itself, but rather the current compiler / tooling process. The compiler currently relies on POSIX / Unix behavior such as:

- shell scripts and `make`
- `unistd`-style APIs like `getcwd(...)`
- `realpath(...)`
- shell commands such as `mkdir -p`
- running produced binaries via `./output/out`

The generated C backend is intended to stay as portable as possible, but the compiler executable and build flow are still Unix-oriented for now.

## Using Flower

```bash
./bin/Flower program.flo
./output/out
```

Flower generates C code, compiles it with Clang, and produces a binary. It will run the binary if there's any output (this is for development purposes).

## Language Features

- **Variables & Types**: `int`, `float`, `double`, `char`, `string`, `bool`, `null`
- **Pointers**: `@int` for pointer to int, `&x` for address-of, `@x` for dereference
- **Control Flow**: `if` / `else`, `while`, `for`, `continue`, `break`, `return`
- **Functions**: Named parameters, explicit types, no overloading (YET)
- **Structs & Unions**: `struct` for grouped data, `union` for raw storage / layout, no methods (use functions instead)
- **Memory**: `new` allocates, `prune` frees
- **Strings**: Equality / inequality, `.length`, indexing and explicit casts to / from `@char`
- **Imports**: Module aliases, exports via prop, private-by-default top-level declarations
- **Advanced Types**: Transparant aliases, semantic nullable types `?T`, semantic unions `A | B`, narrowing with `is`, explicit extraction with `as`

## Experimentation & Contribution

Flower is a platform for compiler research and language design. The whole premise of this project was to see what was possible, and Flower derives its magic from its ability to "branch-off" and be experimented with.

**Try ideas in branches:**

```bash
git checkout -b experiment/optimization-x
# Implement ideas, test, benchmark
# Does it help? Submit a Pull Request! Doesn't? Learn and continue
```

Current experiments welcome. See [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) for specific details on how to contribute.

## Project Status

- **v1.4.xx**: Better types is in active development and largely implemented
- Self-hosting compiler with module-aware typechecking
- Current backend targets C; but Flower semantics are intended to remain compiler-owned rather than C-defined
- Current type surface includes:
 - transparent aliases via `type Name = ExistingType`
 - semantic nullable types via `?T`, lowered as `T | null`
 - semantic unions via `A | B | C`
 - explicit narrowing with `is`
 - explicit extraction with `as`
 - semantic union support across locals, parameters, return types, struct fields, and stable field expressions
- Core language is usable; stdlib is still intentionally small

## Next

- `v1.5.0` Documentation
- `v1.6.0` Standard Library expansion
- `v2.0.0` Non-C codegen

See [docs/ROADMAP.md](docs/ROADMAP.md) for more

## Why Flower?

You want a language that:

- Doesn't hide what's happening,
- Lets you write fast and easily readable code,
- Stays out of your way,
- Doesn't try to appeal explicitly to machines

Flower is that, and much much more
