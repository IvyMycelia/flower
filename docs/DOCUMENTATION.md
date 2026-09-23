# Flower Documentation System

This document defines how Flower documentation is organized, what each document is responsible for, and how documentation should be written going forward.

The goal is not to make the docs feel corporate or sterile, but rather ensure their trustworthiness, uniformality, and completness without losing the tone of Flower's purpose: active development and ever present questioning.

Flower docs should read like a handbook with field notes inside, backed by reference where it matters.

## Core Principles

Flower documentation should aim for:

- truth over polish
- current behavior
- explicit boundaries
- examples that reflect the compiler as it currently exists
- clear separation between language reference, compiler internals, design rationale, and project history

When documentation and the actual compiler disagree, the compiler wins until the docs are corrected.

## Documentation Roles

Flower documentation is divided into four roles.

### 1. Current Language Reference

These documents describe Flower as it currently exists.

They answer:

- what this feature is
- how it looks in source
- what it means
- what rules currently apply
- what is not here yet
- why that missing part is missing

These are the main user-facing language docs. Information regarding actual syntax falls under this category.

### 2. Compiler Documentation

These documents describes the Flower Compiler — herein referred to as FloC — itself.

They answer:

- what this compiler stage does
- what part of the pipeline it owns
- what data or invariants it is responsible for
- what known traps and rough spots exist

These are contributor-facing and implementation-facing docs.

### 3. Design Documentation

These documents explain why Flower is shaped the way it is.

They answer:

- what problem was being solved
- what decision was made
- why that decision was preferred
- what tradeoffs it creates
- what was deferred and why

These are where commentary, drawbacks, purposes, and boundaries are explored most directly.

### 4. History Documentation

These documents record what changed in a milestone or release.

They are historical records, not the primary reference for current language behavior. Consider them an archive, maybe even gospel, but not promises.

## Canonical Documentation Tree

The canonical documentation root is:

```text
docs/
```

The planned structure is:

```text
docs/
    DOCUMENTATION.md
    ROADMAP.md
    CONTRIBUTING.md
    STRUCTURE.md

    language/
        types.md
        functions.md
        control-flow.md
        modules.md
        memory.md
        structs-and-unions.md
        strings.md

    compiler/
        overview.md
        pipeline.md
        lexer.md
        parser.md
        module-loading.md
        typecheck.md
        codegen-c-backend.md
        bootstrap.md

    design/
        philosophy.md
        explicitness.md
        null-and-unions.md
        compiler-vs-backend-semantics.md
        boundaries.md

    Milestones/
        v1.4.0-better-types.md
        v1.5.0-documentation.md

    Releases/
        v1.4.0.md

    Proposals/
        0000-example-propsal.md
```

Not every file above needs to exist right away, but this is the structure new documentation should grow into.

## What Each Area Is For

### `docs/STRUCTURE.md`

This is the front-door handbook for Flower source structure and current surface syntax.

It should describe:

- how Flower source files are organized
- top-level declarations
- general syntax shape
- high-level type surface
- block structure
- basic memory model

It should not try to become the full type-system spec, compiler internals doc, or roadmap.

### `docs/language/`

This is the deeper current reference for language topics.

These pages should document Flower behavior as it exists now.

### `docs/compiler/`

This is the internal reference for FloC itself.

These pages should explain the real compiler pipeline and implementation responsibilities.

### `docs/design/`

This is where rationale lives.

This is the right home for:

- purposes
- tradeoffs
- drawbacks
- boundaries
- rejected alternatives
- deferred work

### `docs/Milestones/`

These files record milestone-level development history and design work.

They should explain what changed in a given milestone, what landed, and what was intentionally deferred.

### `docs/Releases/`

These files are public-facing release notes.

They should be shorter and more ship-oriented than milestone docs. Essentially, summaries whenever a release is.. well, released.

### `docs/Proposals/`

Numbered propsal docs belong here ONLY.

Numbering is appropriate for proposals, but explicitly for them. No main handbook / reference docs should ever be numbered in a similar way.

## Naming Rules

Use:

- lowercase directory names
- lowercase filenames
- kebab-case for topic docs
- versioned names only for milestones and releases
- numbered filenames only for proposals

Good examples:

```text
docs/language/control-flow.md
docs/compiler/module-loading.md
docs/design/null-and-unions.md
docs/Milestones/v1.4.0-better-types.md
docs/Releases/v1.4.0.md
docs/Proposals/0001-pattern-matching.md
```

Avoid:

```text
docs/NewTypes.md
docs/notes2.md
docs/final-final-doc.md
docs/001.md
docs/random-thoughts.md
```

## Writing Voice

Flower docs should feel like:

- a handbook first
- a field journal second
- a reference whenever it is needed

That means the writing should be:

- clear
- direct
- honest
- willing to name rough edges
- willing to say when something is deferred
- willing to say why something is missing

The docs should not read like marketing copy, legal specification text, or empty tutorial filler. That is not Flower's goal.

## Required Sections by Document Type

### Language Docs

```md
# Topic Name

## What This Covers
## Current Surface
## Rules
## Examples
## Notes From The Field
## What Is Not Here Yet
## Why It Is Not Here Yet
## Related Docs
```

### Compiler Docs

Compiler pages should usually contain:

```md
# Compiler Stage

## Purpose
## Position in Pipeline
## What It Owns
## Important Concepts
## Notes From The Field
## Failure Modes
## What Is Not Here Yet
## Why It Is Not Here Yet
## Related Docs
```

### Design Docs

Design pages should usually contain:

```md
# Design Topic

## Problem
## Current Decision
## Why
## Tradeoffs
## Boundaries
## Deferred Work
## Related Docs
```

### Milestone Docs

Milestone pages should usually contain:

```md
# Milestone Name

## Goal
## What Changed
## What Landed
## What Was Deferred
## Notes From The Field
```

### Release Docs

Release pages should usually contain:

```md
# Release Name

## Summary
## Highlights
## Platform Notes
## Verification
## Deferred Follow-Up
```

## Source Of Truth Rules

The docs set should obey these rules:

- current language semantics belong in `docs/STRUCTURE.md` and `docs/language/`
- compiler behavior belongs in `docs/compiler/`
- rationale belongs in `docs/design/`
- milestone docs are historical, not the main reference
- release docs are public summaries, not the main reference
- the roadmap is a planning document, not a language specification
- the README is a front door, not a complete reference

If a topic exists in the compiler but has no dedicated doc yet, that absence should be treated as documentation debt, not that the feature is unofficial. It is in the best interest of Flower and its progress that everything is documented accordingly.

## Boundaries And Deferred Work

Flower docs should explicitly name what is missing.

Every major topic page should say:

- what is not here yet
- why it is not here yet

Those are separate questions.

A feature may be missing because:

- it is genuinely unimplemented
- it is intentionally deferred
- the semantics are still unstable
- the compiler supports part of it but the docs for it are not complete
- it is outside the intended scope of the topic page
- it was forggoten

The docs should name which case applies

## Update Procedure

When a change lands, the following should be reviewed:

- the directly relevant topic page
- `docs/STRUCTURE.md` if the surface changed broadly
- the relevant compiler page if the implementation model changed
- the relevant design page if the rationale or boundaries changed
- the milestone doc if the change belongs to an active milestone
- the roadmap if milestone scope changed
- the README if the public summary changed

The goal is to keep drift small and obvious instead of allowing it to pile up and become an even bigger problem.

## Drift Checklist

Before considering a documentation page done, check:

- does this page describe current behavior
- do the examples still match Flower syntax today
- is this current-reference material, or does it actually belong in history / design docs
- does this page clearly name boundaries
- does this page say what is missing
- does this duplicate another page unnecessarily
- are internal links using lowercase `docs/` paths

## What Not To Do

Do not:

- use numbered files as the main documentation system
- keep duplicate documentation trees
- mix history, current reference, and roadmap planning without clear boundaries
- leave stale examples in current reference docs
- create random catch-all markdown files with no defined role
- hide deferre work as if it does not matter

## Current Documentation Milestone Meaning

`v1.5.0` should produce the complete docs set needed to understand the current Flower language and compiler.

That includes:

- language handbook pages
- compiler documentation
- design / rationale pages
- uniform documentation rules
- drift cleanup across existing public docs

The purpose of this milestone is not to make the docs feel finished forever. It is to make them coherent, structurally sound, and provide a base for future documentation.
