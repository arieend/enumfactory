---
name: test-gen
description: Generate focused C tests for EnumFactory enums in test/enumfactory_test.c. Use when a new enum or enum map is added or changed to verify values, count/range semantics, labels, to_string behavior, and safety macros.
---

# Test Generation Skill

Add or update one test function per enum change in `test/enumfactory_test.c`.

## Required assertions

1. Value assertions
- Verify each critical enum member value.
- For assigned enums, verify sparse values explicitly.

2. Count and range assertions
- Assert `ENUM_COUNT(<ENUM>)` equals logical member count.
- Assert `ENUM_TOTAL(<ENUM>)` equals max-value-plus-one range end.

3. String/label assertions
- Call `ENUM_TO_STRING(<ENUM>)` once in scope if needed.
- Assert `<ENUM>_to_string(value)` and/or `<ENUM>_get_label(value)` for representative members.

4. Safety assertions
- Verify `ENUM_IS_VALID(<ENUM>, valid_value)` is true.
- Verify negative, gap, and out-of-range values are false.
- If map arrays exist, verify `ENUM_SAFE_ARRAY_ACCESS` returns expected value for valid index and `NULL`/zero for invalid index.

## Test template

```c
void test_status_enum(void) {
    ENUM_TO_STRING(STATUS);

    assert(STATUS_OK == 200);
    assert(STATUS_NOT_FOUND == 404);

    assert(ENUM_COUNT(STATUS) == 2);
    assert(ENUM_TOTAL(STATUS) == 405);

    assert(strcmp(STATUS_to_string(STATUS_OK), "STATUS_OK") == 0);
    assert(strcmp(STATUS_get_label(STATUS_NOT_FOUND), "STATUS_NOT_FOUND") == 0);

    assert(ENUM_IS_VALID(STATUS, 200));
    assert(!ENUM_IS_VALID(STATUS, 201));
    assert(!ENUM_IS_VALID(STATUS, -1));
    assert(!ENUM_IS_VALID(STATUS, 405));
}
```

## Implementation rules

- Use deterministic function names: `test_<enum_name_lower>_*`.
- Keep assertions narrow and behavior-oriented.
- Prefer adding to existing test sections instead of creating duplicate coverage.
- Include at least one boundary assertion for every changed enum.
