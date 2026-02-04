---
name: macro_doc
description: Detailed documentation for EnumFactory macro generation
---

# Macro Documentation Skill

This skill ensures that whenever the EnumFactory library is used to generate enums or maps, a detailed comment block is provided to explain the resulting values and structures.

## Usage Guidelines

When you (the agent) generate an enum using `ENUMS_AUTOMATIC`, `ENUMS_ASSIGNED`, or `ENUMS_MAP`, you MUST include a comment block immediately following the generation.

### Comment Format

```c
/*
 * Generated Enum: [EnumName]
 * -------------------------
 * Type: [Automatic | Assigned | Map]
 * Actual Member Count: [Count]
 * Range (total): [0 to Total-1]
 *
 * Members & Values:
 * - [Member1] = [Value1] ([Label1])
 * - [Member2] = [Value2] ([Label2])
 * ...
 */
```

### Example

Input:

```c
#define COLOR_ENUM(GENERATOR) \
    GENERATOR(RED), \
    GENERATOR(BLUE)
ENUMS_AUTOMATIC(COLOR)
```

Output with Skill:

```c
#define COLOR_ENUM(GENERATOR) \
    GENERATOR(RED), \
    GENERATOR(BLUE)
ENUMS_AUTOMATIC(COLOR)
/*
 * Generated Enum: COLOR
 * -------------------------
 * Type: Automatic
 * Actual Member Count: 2
 * Range (total): 0 to 2 (exclusive)
 *
 * Members & Values:
 * - RED = 0 ("RED")
 * - BLUE = 1 ("BLUE")
 */
```

## Rationale

Since X-macros expand during compilation, they are "invisible" in the source code. This documentation makes the intended expansion explicit for developers, improving code readability and debugging.
