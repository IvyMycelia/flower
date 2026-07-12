# Modules

## What This Covers

This document contains information pertaining to Flower's current module and import system.

That includes:

- what a module is
- file imports
- system imports
- aliases
- exported declarations via `prop`
- what privacy is currently enforced
- cross-module field visibility
- path resolution rules
- rough edges and deferred module behavior

The module system is quite new relatively speaking, and is changing often. To avoid confusion, this document goes over explicitly what works *today*, not how a future cleaned-up module system might look like.

## Current Surface

Flower currently supports these module-related forms:

### File Imports

```flo
import "exaples/import/math.flo" as math
import "./local/util.flo" as util
import "../shared/path.flo"
import "src/file.flo"
```

### System Imports

```flo
import <stdio>
import <stdlib>
import <string>
import <ctype>
import <unistd>
```

### Exported Functions

```flo
prop func add(a: int, b: int): int
    return a + b
end
```

### Internal Helpers

```flo
func helper(): int
    return 0
end
```

### Exported Structs and Unions

```flo
prop struct User {
    id: int
}

prop union Bits {
    i: int,
    f: float
}
```

### Field Flags

```flo
prop struct User {
    hidden password: string,
    readonly id: int,
    username: string
}
```

## A Module Is A File

In current Flower, a module is simply another source file.

There is no separate `module` keyword, no package declaration, and no explicit namespace declaration. The file itself is the module boundary. All files are considered modules due to this, and any `.flo` file can be imported into any other `.flo` file.

That means this:

```flo
import "examples/import/math.flo" as math
```

is not importing a named package object. It is importing another Flower file and giving it an alias that the current compiler can use for certain forms of cross-module access.

Practically, that means:

- each `.flo` file is a module
- top-level declarations belong to that file's module surface
- imports should appear before code that uses their aliases
- there is no separate module declaration block

Flower's parser currently learns aliases as it reads imports, so import order affects what later syntax can be recognized cleanly, not just style.

## File Importing

Flower currently has two import families:

- file imports
- system imports

They look similar at first glance, but they do not mean the same thing.

### Files

File imports loads another Flower soruce file.

```flo
import "examples/import/math.flo" as math
```

This is the ordinary Flower module form.

A file import can currently be written with or without an alias:

```flo
import "src/file.flo"
import "src/file.flo" as file
```

But the stable and intended style right now is to use an alias whenever you mean to call something through the imported module.

```flo
import "examples/import/math.flo" as math

value: int = math.add(1, 2)
```

### Path Resolution

Quoted import paths follow the following copiler path rules:

If the path begins with `./` or `../`, it is resolved relative to the importing file:

```flo
import "./helpers/math.flo" as math
import "../shared/path.flo" as path
```

If the path does not have `./` or `../`, it is resolved from the project root:

```flo
import "src/file.flo" as file
import "examples/import/math.flo" as math
```

So current Flower path behavior is closer to:

- relative when the path explicitly says so
- project-root based otherwise

That is the current compiler rule, not just my preference whilst writing these docs.

### Imports Are Top-Level

Imports are currently a top-level declaration form.

That means they belong at module scope, not inside functions or control-flow blocks.

This is also why the specific current parser behavior matters: aliases are registered as imports are parsed, so in practice, put imports near the top of the file before anything that uses them.

### Recursive Loading

The current module loader resolves non-system imports recursively.

That means when one module imports another, Flower loads that imported file, parses it, collects its exports, and then continues walking imports from there.

The same file is only loaded once by path, even if it is imported multiple times. That helps the current compiler avoid duplicating module state, and it means repeated imports are treated more like repeated references than separate instances. This is in-part due to necessity, not just preference, as currently source files are transpiled to a singular C file.

## System Importing

System imports are different.

```flo
import <stdio>
import <stdlib>
```

A system import does **not** load another Flower source file. Instead, it is a backend-facing import request.

In the current C backend, system imports are used to request generated C includes such as:

- `stdio`
- `stdlib`
- `string`
- `ctype`
- `unistd`

So when you write:

```flo
import <stdio>
```

you are not importing a Flower module named `stdio`. You are asking the backend to make the corresponding system surface available.

That is why system imports feel different from file imports: they are backend-shaped, not ordinary language modules. For that reason, they are currently a dependency Flower needs so long as it lacks its own standard library to rely on. Until then, system imports are needed to keep C-compatibility.

### Backend-Dependent Surface

System imports are not a general Flower module system. They are part of the current backend bridge. This means that names used with them, such as:

```flo
printf(...)
fprintf(...)
snprintf(...)
getenv(...)
```

are not ordinary Flower function declarations defined in a `.flo` file. They are handled especially by the compiler and backend.

### System Import Aliases

The parser currently accepts alias syntax after system imports:

```flo
import <stdio> as stdio
```

But this is not a real general-purpose Flower module namespace in the current compiler.

Do not treat system import aliases as a stable lnguage surface. They do not work the same as ordinary module alias calls.

## Aliases

Aliases are the current explicit handle for imported Flower modules.

```flo
import "examples/import/math.flo" as math
```

That gives you the alias `math`, while the parser and typechecker can recognize later for imported function calls.

### Alias Calls

The currently supported alias call form is:

```flo
math.add(1, 2)
```

This is not just a visual convention, it is a real AST form in the compiler.

Flower currently has dedicated handling for alias-based function calls, and this is where exported function checks actually happen.

### What Aliases Currently Work For

Right now, aliases are reliable for:

- calling exported functions from another file

That is the main important part — and technically one of the only parts.

Aliases are **not yet** a complete general namespace system.

There is currently no clean, documented namespaced type syntax such as:

```flo
math.Vector
logs.User
utils.Value
```

That means the current alias system is more accurately described as:

- function-call namespace support

not a full namespace semantics for every declaration kind.

## Exports With `prop`

`prop` is the how top-level declarations are marked as public surface.

### Function Exports

The strongest and most complete current use of `prop` is with functions.

```flo
prop func add(a: int, b: int): int
    return a + b
end
```

An alias call to an imported function checks whether that function is exported.

So if another file contains:

```flo
func helper(): int
    return 0
end

prop func add(a: int, b: int): int
    return a + b
end
```

then:

```flo
math.add(1, 2)
```

is the intended public call, while:

```flo
math.helper()
```

is rejected by the alias-call export checks.

That's one of the best examples of Flower's module system being genuinely semantic, not just stylistic.

### Struct and Union Exports

Flower also parses and records:

```flo
prop struct User {
    id: int
}

prop union Bits {
    i: int,
    f: float
}
```

The module system tracks these as exports.

However, the broader type-namespace side of this is not yet finished.

So while `prop` struct and `prop union` clearly mark public intended declarations, Flower does not yet have a clean imported type namespace like:

```flo
users.User
bits.Bits
```

Instead, they'd be accessed directly with no alias dot access. This is not hard-coded into the compiler, just a side effect of relying on a singular C generated file as output.

That means these exported forms exist and matter, but their cross-module usability is still rougher than their function counterparts.

### `prop type`

Flower currently parses `prop type`:

```flo
prop type Count = int
```

But type aliases are not currently a real imported / exported module surface.

In practice today, type aliases remain local to their source file's type resolution path. They are not the same kind of public imported surface that `prop func` currently is. This means that using an imported type alias may not work due to the typechecker being unable to properly resolve what type it is an alias for and therefore the codegen not emitting proper C.

So `prop type` exists in syntax, but should not be relied on as a finished cross-module feature.

### Top-Level Variables

Flower does not currently support top-level variables through `prop`. You can access top-level variables directly, just as you would with unions and structs, but they are not enforced at the typecheck level.

`prop` is currently used with:

- `func`
- `struct`
- `union`
- `type`

If you want a public callable surface, think in terms of exported functions, not exported globals.

## Privacy and The Current Reality

The intended Flower direction is private-by-default top-level declarations with explicit public access through `prop`.

That direction is visible in the compiler, but the current implementation still has many gaps and rough edges.

### The Stable Part

These parts are meaningfully enforced:

- alias calls to imported functions require the target function to be exported
- `hidden` fields are blocked outside the defining module
- `readonly` fields reject external assignment

This is the part of the module model that is already genuinely semantic and not just syntax.

### The Loose Part

Other name resolution paths are still looser than the intended model.

### Unqualified Function Lookup Is Still Global By Name

Plain function calls like:

```flo
add(1, 2)
```

are currently resolved by name across the loaded compiler environment, not strictly through alias imports.

That means unqualified function lookup is still technically allowed and not enforced, despite what the cleaner module model appears to do.

Now, despite this technicality, unlike structs and unions and variuables, function names are resolved to a file path format when generated to the C file, so to actually call a non-imported function requires an extreme-case work around — one that I'm not sure how would work, but probably possible.

So while this is the intended public style:

```flo
import "examples/import/math.flo" as math

value: int = math.add(1, 2)
```

the current compiler still has rough edges around global bare-name lookup.

Practical rule: use alias calls for intentional cross-module public access.

### Struct and Union Names Are Not Yet Cleanly Namespaced

Struct and union names are also not yet part of a finished imported namespace system.

That means type names are still much closer to:

- bare-name resolution
- global uniqueness pressure
- undefined behavior if multiple files define the same name

than to a polished namespace system.

The practical rule is to keep cross-module type names unique and do not assume `alias.Type` exists yet. I've had to remind myself quite a bit as I've been working on the compiler, so trust me when I say it's important.

### Type Aliases Remain Local

Type aliases currently resolve within their defining source context rather than through a finished imported module-alias mechanism.

So while aliases are part of Flower's type implementation, they are not yet part of Flower's mature public module system.

## Cross-Module Field Visibility

The more real module-boundary feature Flower has is field visibility control. Besides this, there aren't many features that control specific visibility yet.

Flags that currently exist are:

- `hidden`
- `readonly`
- `frozen`

They are written before the field name:

```flo
prop struct User {
    hidden password: string,
    frozen id: int,
    readonly username: string
}
```

Flags are modular, meaning you can apply more than one to a singular field. Flower follows a cascading privacy model. That means if a struct is exported via `prop`, all fields will be exported and public by default.

The same flag parser is used for both structs and raw storage unions.

### `hidden`

A `hidden` field is treated as unavailable outside its defining module.

From another module, the compiler does not present it as a normal accessible field. The external access error is intentionally phrased more like "this field does not exist here" than "you are not allowed" as to preserve confidentiality and privateness.

### `readonly`

A `readonly` field can still be read, but external assignment is rejected.

That means another module may be able to observe the value, but not write to it.

This is, in my opinion, one of the cleanest current examples of Flower enforcing a module boundary semantically instead of leaving it to backend coincidence.

### `frozen`

`frozen` currently exists as groundwork, but is it not implemented yet as a proper language feature.

So while you may use it and it is supported, you should not rely on it in any capacity to do its job.

Ideally, you would write to it *once* and then it'd become frozen. However currently it does not do anything.

## Examples

### Exported Function Surfac

`math.flo`:

```flo
prop func add(a: int, b: int): int
    return a + b
end

func helper(): int
    return 0
end
```

`main.flo`:

```flo
import "math.flo" as math

func main(): int
    return (math.add(a, 2) == 3) as int
end
```

Current intended behavior:

- `math.add(...)` is valid
- `math.helper(...)` is not publicly accessible

### Relative And Project-Root Imports

```flo
import "./helpers/math.flo" as math
import "../shared/path.flo" as path
import "src/file.flo"
```

Current rule:

- `./` and `../` are resolved relative to the importing file
- plain quoted paths are resolved from project root

### Cross-Module Field Boundaries

`user.flo`:

```flo
prop struct User {
    readonly id: int,
    hidden password: string,
    username: string
}

user_count: int

prop func make_user(username: string, password: string): @User
    user: @User = new User

    user.password = password
    user.username = username

    user.id = user_count + 1
    user_count = user_count + 1

    return user
end
```

`app.flo`:

```flo
import "user.flo" as users

func touch(user: @User): void
    user.username = "Ivy"
    // valid: username is public and writeable from external scope

    user.id = 2
    // invalid: readonly from external scope

    // print(user.password)
    // invalid: hidden outside the defining module
end

prop func test(): void
    user: @User = users.make_user()
    touch(user)
    prune user
end
```

Notice the current shape here:

- function access uses the alias
- type access is still bare-name based, not `users.User`

That is not an accident in the example, it reflects the current compiler's abilities.

## Notes From The Field

Flower's current module system is in an interesting middle stage.

It is not Just "throw everything at C and pray names work out" anymore. Now the compiler really does load modules, track exports, resolve aliases, and enforce some boundaries. Alias-based exported function calls are real, cross-module field restrictions are real, those parts got teeth.

But the whole system is not uniformly strict yet.

Some declaration kinds still behave more like  global names than fully namespaced imported members. Type aliases are not finished. Struct and union names do not yet live behind an alias namespace. Unqualified function lookup is still looser than the intended design.

So the best way to think about Flower modules as of right now is:

- functions and their visibility enforcement is becoming more semantically real
- field visibility is just starting to sprout
- full namespace features are not yet there

## What Is Not Here Yet

The following are not part of the current module system:

- `from x import y`
- imported member selection, e.g: `import x::{y, z}`
- namespaced type references like `math.Vector`
- exported top-level variables / constants
- strict module-scoped unqualified function lookup
- strict module-scoped unqualified type lookup
- a fully implemented `frozen` field contract
- backend-independent system import semantics
- explicit package or module declaration blocks

## Why It Is Not Here Yet

Flower's module system grew out of several practical needs at once:

- self-hosted compilation
- recursive loading before codegen
- explicit public API
- import-aware typechecking
- field access enfrocement across files

It already has some real enforcement in certain places, but its global namespace system and import checking remains largely unimplemented / unfinished.

## Related Docs

- [Types](../language/types.md)
- [Functions](../language/functions.md)
- [Control Flow](../language/control-flow.md)
- [Project Structure](../STRUCTURE.md)
- [Roadmap](../ROADMAP.md)
