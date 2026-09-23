# Flower Versioning

Versions should be written as follows:

```text
MAJOR.MINOR.PATCH
```

Examples:

```text
1.4.0
1.2.4
2.0.0
```

Git tags should use a `v` prefix:

```text
v1.4.0
v1.2.4
v2.0.0
```

A version should tell you what type of change you are looking at:

- new stages of the language
- some meaningful change
- a correction to behavior
- a release that requires updating part of the codebase

This isn't really here to give off the impression of formality, rather its purpose it to create some form of traceable system.

## MAJOR

The `MAJOR` number should increase when the new version is incompatible with the prior ones. Some examples include:

- syntax change
- a change to the compiler pipeline
- semantical change

Essentially any change that causes the bootstrap process to fail on the first run.

For example, when I changed the syntax for function declarations, previous programs became uncompatible with the new compiled version and thus I had to manually bootstrap it. For example, that could look like:

```text
1.9.0 —» 2.0.0
```

## MINOR

When a change that doesn't cause incompatible binaries / compilers, typically the `MINOR` number should be changed. This is ideally the most common kind of release, given how almost all work on the project falls under new features and less-structural changes to the compiler.

A few examples of what might constitute a Minor release could be:

- new language features
- new behavior
- new syntax
- new compiler checks
- new standard library modules or functions
- intentional breaking changes
- changes to the bootstrap or testing workflow
- documentation, examples, or other non-compiler changes

If a new `math.flo` library were to be created and released alongside the compiler itself, for example, it might look like:

```text
1.4.0 —» 1.5.0
```

## PATCH

``PATCH` is explicitly for a correction rather than an expansion or general change. Stuff like:

- bug fixes
- test fixes
- platform / build fixes
- cleanup

Example:

```text
1.5.2 —» 1.5.3
```

A patch release may still break someone's code, if the code compiled only due to a bug that was present. That's not necessarily a contradiction, but rather fixing acceptance of invalid input when it shouldn't happen.

## Compatability

When it comes to "Compatibility," it means a few separate things that are best described as 'layers.'

### Source Compatability

**Will the same source code still compile?**

That's one of the simpler layers, since it has a direct answer. Source can break from:

- syntax changes
- new restrictions
- renamed standard library APIs
- stricter type rules
- visibility or module rule changes

etc etc etc.

There's no promise for source compatibility across all of `1.x`, however there is a strong intention to maintain backwards and forwards compatibility.

### Semantic Compatibility

**Does the same program still result in the same thing?**

This is a bit more difficult to answer, since a release may keep source code compiling whilst still changing things such as:

- narrowing behavior
- alias resolution behavior
- union behavior
- null handling
- codegen output
- standard library behavior

As evident by the common theme, if behavior changes intentionally, it should be considered a 'breaking change,' even if the syntax remains the same.

### Tooling Compatability

Not necessarily a language change, bit still 