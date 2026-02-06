# Agent Instructions for EnumFactory

## Project Overview

EnumFactory is a header-only C++ library that automates enum generation using X-Macros. It provides string conversion, iteration, and safety checks without runtime overhead.

## Key Files

- `enumfactory/include/enumfactorymacros.h`: Core library file.
- `test/enumfactory_test.c`: Comprehensive test suite and usage examples.

## Coding Guidelines

### 1. Enum Definition Pattern

Enums are defined as macros to allow multiple expansions (X-Macro pattern).

**Syntax:**

```c
#define NAME_ENUM(X, G) \
    X(G, MEMBER_1 [, VALUE]) \
    X(G, MEMBER_2 [, VALUE])
```

_Note: Do not use trailing commas or semicolons inside the macro definition._

### 2. Generation Macros

- `ENUMS_AUTOMATIC(NAME)`: Generates `enum NAME` with sequential values (0, 1, ...).
- `ENUMS_ASSIGNED(NAME)`: Generates `enum NAME` with explicit values.
- `ENUMS_MAP(...)`: Generates parallel arrays (e.g., string descriptions).

### 3. Documentation Requirement

When writing code that generates an enum, you **MUST** include a comment block detailing the generated members and values. This aids readability because macros hide the actual definitions.
_Refer to `.agent/skills/macro_doc/SKILL.md` for the specific format._

### 4. Test Requirement

When adding a new enum, you **MUST** add a corresponding test function in `test/enumfactory_test.c`.
_Refer to `.agent/skills/test_gen/SKILL.md` for the specific pattern._

### 5. Safety and Best Practices

- Use `ENUM_IS_VALID(NAME, value)` to validate integers before casting to enum types.
- Use `NAME_to_string(value)` for logging/debugging.
- Use `ENUM_SAFE_ARRAY_ACCESS` when retrieving mapped values.
- Use `ENUM_COUNT` for the actual member count and `ENUM_TOTAL` for the range end.

## Build & Test

- **Build System**: Makefile
- **Commands**:
  - `make`: Builds the test executable.
  - `make test`: Runs the test suite.
  - `make clean`: Removes artifacts.
