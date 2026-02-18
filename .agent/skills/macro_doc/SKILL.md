---
name: macro-doc
description: Generate deterministic inline documentation blocks for EnumFactory X-macro expansions. Use when adding or modifying enums built with ENUMS_AUTOMATIC, ENUMS_ASSIGNED, ENUMS_ARRAY, or ENUMS_MAP so generated values, count, and range are explicit in source.
---

# Macro Documentation Skill

Create a comment block immediately after every enum generation macro call.

## Apply this output format

```c
/*
 * Generated Enum: <ENUM_NAME>
 * Type: <Automatic|Assigned|Map>
 * Actual Member Count: <N>
 * Range (total): 0 to <ENUM_total - 1> (exclusive upper bound: <ENUM_total>)
 *
 * Members & Values:
 * - <MEMBER_A> = <VALUE_A> ("<LABEL_A>")
 * - <MEMBER_B> = <VALUE_B> ("<LABEL_B>")
 */
```

## Compute values correctly

- Use `ENUM_COUNT(<ENUM>)` for `Actual Member Count`.
- Use `ENUM_TOTAL(<ENUM>)` for range end (`exclusive upper bound`).
- For assigned enums, include sparse values exactly as declared.
- For map arrays (`ENUMS_ARRAY`/`ENUMS_MAP`), add mapped payload when relevant:

```c
 * - <MEMBER> = <VALUE> (Score: <MAPPED_VALUE>)
```

## Placement rules

- Place the block directly under `ENUMS_AUTOMATIC(...)`, `ENUMS_ASSIGNED(...)`, or `ENUMS_MAP(...)`.
- Keep one block per generated enum.
- Keep wording stable to reduce diff noise.

## Minimal example

```c
#define STATUS_ENUM(X, G) \
    X(G, OK, 200) \
    X(G, NOT_FOUND, 404)

ENUMS_ASSIGNED(STATUS)
/*
 * Generated Enum: STATUS
 * Type: Assigned
 * Actual Member Count: 2
 * Range (total): 0 to 404 (exclusive upper bound: 405)
 *
 * Members & Values:
 * - OK = 200 ("OK")
 * - NOT_FOUND = 404 ("NOT_FOUND")
 */
```
