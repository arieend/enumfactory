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
*Note: Do not use trailing commas or semicolons inside the macro definition.*

### 2. Generation Macros
- `ENUMS_AUTOMATIC(NAME)`: Generates `enum NAME` with sequential values (0, 1, ...).
- `ENUMS_ASSIGNED(NAME)`: Generates `enum NAME` with explicit values.
- `ENUMS_MAP(...)`: Generates parallel arrays (e.g., string descriptions).

### 3. Documentation Requirement
When writing code that generates an enum, you **MUST** include a comment block detailing the generated members and values. This aids readability as macros hide the actual definitions.
*Refer to `skills/macro_doc/SKILL.md` for the specific format.*

### 4. Safety & Best Practices
- Use `ENUM_IS_VALID(NAME, value)` to validate integers before casting to enum types.
- Use `NAME_to_string(value)` for logging/debugging.
- Use `ENUM_SAFE_ARRAY_ACCESS` when retrieving mapped values.

## Build & Test
- **Build System**: Makefile
- **Commands**:
  - `make`: Builds the test executable.
  - `make test`: Runs the test suite.
  - `make clean`: Removes artifacts.
