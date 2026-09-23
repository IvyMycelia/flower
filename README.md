# Flower 🌸

This is a small programming language I am making! I was fairly turned off by the way other languages chose to handle semantics, syntax decisions, and various language features.

For example, C has a very small syntax _ which I like _ but actually putting it to use gets complex FAST. It's explicit which I prefer, but covers a lot of behavior behind complicated tooling and features, making it some form of 'necessary evil.'

On the other hand, Rust's syntax is **large**, and a lot of things need to be explained deliberately through documentation because of how interwoven everything is. A new programmer might eb able to understand the basics of what C's syntax means despite its complex usages, whereas Rust's over-complicated ways to do primitive things in the language makes it difficult.

Flower is something in the middle: it has a smaller syntax (though bigger than C), simple semantics that can be intuitively deduced by even beginners, whilst retaining the power of C and _ eventually _ the safety of Rust.

Most importantly, though, it's meant to be an experiment. It's difficult to escape the typical language paradigm, but Flower puts emphasis on trying out new things! More information on this can be found in the [Contributing](docs/CONTRIBUTING.md] guide.

## A Little Flower

```flo
func factorial(n: int): int
    if n <= 1:
        return 1
    end

    return n * factorial(n - 1)
end

func main(): int
    print("5! =")
    print(factorial(5))
    return 0
end
```

It currently reads fairly C-esque, however this will most likely change. As stated prior, Flower's purpose is to be full of whatever whimsy people find helpful and lacking in other languages, which ~~naturally~~ means it'll slowly drift away from C's influence.

## Where It's ~~Go~~Growing

The Flower compiler _ herein referred to as "FloC" _ is written in its own source code, meaning the majority of the compiler process is bootstrapped.

This means that in order for it to work effectively as not only a compiler but its own one as well, it must support features such as:

* functions with typed parameters and return values
* modules, alias imports, and private-by-default top-level declarations
* structs and storage unions
* built-in `string` and `bool` types
* transparent type aliases
* `null` and nullable types with `?T`
* semantic unions with `A | B`
* type narrowing with `is`
* extraction and conversion with `as`
* manual allocation and cleanup with `new` and `prune`

Though all of these features do exist and have been thouroughly tested, there's still some bare patches. For example, not every logic path is covered with the automatic narrowing system.
The standard library is small, C is currently the only backend, Windows-oriented development is rougher than I'd prefer, and broader tooling and reference documentation are but tiny little seedlings.

Due to the experimental nature of Flower, things in the roadmap are **not promised**. Some are planned work, others experiments, and many may be pruned before they ever get a chance to 'bloom', so-to-speak (Yes, pun was intended <3).

## Why Flower?

Mostly because I wanted to make it.

It's an experiment in what a low-level language could look like without being absolutely abysmally miserable to write and retaining the same capabilities.

I care about things like:

- Being able to understand a program
- Writing code that feels and looks nice
- Not having to worry about vague rules and meanings
- Having a guide that's the only thing needed to learn
- Actual features that matter to its goals, not fluff
- Honesty
- Being whimsical, of course! I love whimsy :>
- And code being readily sharable

Of course, there's many more principals that go into this than just what's been listed above, but these are the core ones at the tip of the ol' cranium.

It probably goes without saying, but I'm not trying to disguise Flower as a finished industrial language _ hell, I don't even want it to be! The compiler and language are being developed together, thus, breaking changes are normal (and expected) :)

## Trying Out Flower

There is a bootstrap C snapshot kept in `bin/Flower.c` for those who wish to compile Flower.

To build and verify the compiler:

```bash
make build
make bootstrap
make test
```

To compile a Flower program directly:

```bash
./bin/Flower ./examples/test.flo ./optional_output_path
```

If you do not provide an output path, it will automatically output to `/output/out.c` and `/output/out`.

For more information on using specific Flower arguments, you can use `./bin/Flower -h` to see the help information!

## Building Flower

Flower is still primarily a development project rather than something I'd recommend installing system wide.

The exact compiler-development and bootstrap workflow is documented in [CONTRIBUTING.md](./CONTRIBUTING.md).

If you're modifying the compiler, make sure to run the relevant tests and bootstrap checks before trusting the result. A compiler successfully compiling *something* is, unfortunately, not very strong evidence that it actually works!

## Trying Flower

There is a bootstrap C snapshot kept in `bin/Flower.c`.

To build and verify the compiler:

```bash
make build
make bootstrap
make test
```

To compile a Flower program directly:

```bash
./bin/Flower ./examples/test.flo
```

## Project layout

The respository is centered around a few parts:

```text
Flower
Flower
├── src         compiler source written in Flower
├── include     shared declarations used by the compiler (will be phased out)
├── examples    language and runtime examples
├── docs        project documentation and roadmap
├── bin         bootstrap snapshot and built compiler artifacts
├── scripts     build helpers
├── vendor      original C implementation
└── ...
```

The layout may continue to change as the compiler is worked on.

## Roadmap

Current priorities and possible future plans reside in [ROADMAP.md](./ROADMAP.md).

The short version is that I'm much more interested in making the existing compiler solid than accumulating a giant pile of half-finished features, and the roadmap should (ideally) represent this.

Thus, near-term work generally revolves around things like compiler correctness, self-hosting, the parser and type system, code generation, diagnostics, the standard library, and the parts that are still awkward to use in real programs.

Farther-out ideas are treated as experiments until I actually attempt to tackle them, which can take any undetermined amount of time :p

## Contributing

Flower is small enough that contributions can have a very visible effect, and I appreciate all the help I can get!

Bug reports, tests, compiler fixes, documentation corrections, and carefully considered language work are all useful.

If you'd like to dig into the compiler, I suggest taking a look at [CONTRIBUTING.md](./CONTRIBUTING.md)!

You do not need to create some grand redesign of the language — pulling one weed is useful too, and I appreciate every effort :>

## Documentation

The documentation is fairly non-existent currently.

For now, the repository itself is the source of truth for what the compiler actually does. Documentation will be added alongside the language ~~whenever I get to it~~.

If the compiler and roadmap idea disagree, the compiler is probably right.

If the compiler and the documentation disagree, that's most likely a bug!

## Stability

There isn't much to say to be honest.

Not to repeat myself, this project is under active development, thus syntax, semantics, compiler behavior, and APIs may change between revisions.

If you're using Flower right now, you're using a language while it's being radically worked on rather than after it has stabilized.

Personally for me, that's part of the fun ;)

## License

See [LICENSE](./LICENSE).
