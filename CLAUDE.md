# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Test

```sh
make          # builds libenumfactory.a and enumfactory_test binary
make test     # runs the test suite
make clean    # removes build artifacts
```

Compiler: `gcc -I./include -Wall -Wextra -O2`. The static library is linked via `-L. -lenumfactory`.

There is no way to run a single test in isolation — `make test` runs all tests.

## Architecture

EnumFactory is a header-only C preprocessor library (`include/enumfactorymacros.h`). `src/enumfactory.c` contains only a stub `enumfactory_init()` so the library can be packaged as a `.a` archive.

### X-Macro Pattern

All enum metadata derives from a single macro list defined by the user:

```c
#define NAME_ENUM(X, G) \
    X(G, MEMBER_1 [, VALUE [, attr2, ...]]) \
    X(G, MEMBER_2 [, VALUE [, attr2, ...]])
```

`X` is the generator and `G` is a context token threaded through. The same list is expanded multiple times with different generators to produce different outputs — no manual synchronization needed.

### Generation Macros

| Macro | What it generates |
|---|---|
| `ENUMS_AUTOMATIC(NAME)` | `enum NAME` with sequential values (0, 1, …), plus count/label/accessor |
| `ENUMS_ASSIGNED(NAME)` | `enum NAME` with explicit values from the list |
| `ENUMS_ARRAY(NAME, LIST, type, suffix, [index])` | `NAME_get_<suffix>(int)` pulling column `index` (0–10) from each member's attribute tuple |
| `ENUMS_MAP(NAME, LIST, generator, type, suffix)` | Enum + primary mapped function in one call |
| `ENUM_TO_STRING(NAME)` | `NAME_to_string(int)` label accessor |

### What Every Enum Gets

Each `ENUMS_AUTOMATIC` / `ENUMS_ASSIGNED` / `ENUMS_MAP` call produces:

- `typedef enum { … } NAME;`
- `static const int NAME_count` — number of members
- `static inline const char* NAME_get_label(int)` — safe label getter (NULL on out-of-bounds)

### Multi-Attribute Arrays

Tuple attributes beyond the name/value are extracted by position with `ENUMS_ARRAY(… index)`. Up to 10 attributes per member (indices 0–10) are supported. Each call generates a typed getter function `NAME_get_<suffix>(int value)` backed by a `switch` jump table with O(1) performance.

### Safety Utilities

- `ENUM_IS_VALID(NAME, value)` — validate an integer before casting
- `ENUM_COUNT(NAME)` — member count constant
- `ENUM_TOTAL(NAME)` — highest value + 1 (use for range iteration, not count)
- `ENUM_SAFE_ARRAY_ACCESS(array, enum, index)` — bounds-checked array index

## Coding Rules (from INSTRUCTIONS.md)

1. **Comment block required**: Whenever you generate an enum with a macro, add a comment documenting the generated members and their values. Macros hide definitions so this is essential for readability.

2. **Test required**: Every new enum must have a corresponding test function added to `tests/enumfactory_test.c`.

3. **No trailing commas or semicolons** inside `#define NAME_ENUM(X, G)` macro bodies.

4. Prefer `ENUM_IS_VALID` before casting raw integers to enum types. Prefer `NAME_to_string()` for debug output over raw integer printing.
