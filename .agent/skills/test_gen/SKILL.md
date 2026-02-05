---
name: test_gen
description: Standard pattern for testing EnumFactory enums
---

# Test Generation Skill

This skill ensures that new enums are accompanied by comprehensive tests in `test/enumfactory_test.c`.

## Test Requirements

When adding a new enum, create a `void test_enum_name_lower` function that validates:

1. **Values**: Assert specific enum members match expected integers.
2. **Count**: Assert `[ENUM]_total` and `ENUM_COUNT([ENUM])` are correct.
3. **Strings**: Assert `[ENUM]_to_string` or `[ENUM]_label` returns correct strings.
4. **Safety**: Use `ENUM_IS_VALID` and `ENUM_SAFE_ARRAY_ACCESS` to test boundary conditions.

## Template

```c
void test_new_enum(void) {
    // Value checks
    assert(MEMBER_1 == 0);
    assert(NEW_ENUM_total == 2);
    assert(ENUM_TOTAL(NEW_ENUM) == 2);
    assert(ENUM_COUNT(NEW_ENUM) == 2);

    // String checks
    // Prefer _to_string or _get_label accessor over direct array access
    assert(strcmp(NEW_ENUM_to_string(MEMBER_1), "MEMBER_1") == 0);
    assert(strcmp(NEW_ENUM_label[MEMBER_1], "MEMBER_1") == 0); // optional direct access check

    // Safety checks
    assert(ENUM_IS_VALID(NEW_ENUM, MEMBER_1));
    assert(!ENUM_IS_VALID(NEW_ENUM, -1));
    assert(!ENUM_IS_VALID(NEW_ENUM, NEW_ENUM_total));

    // String Conversion Macro
    ENUM_TO_STRING(NEW_ENUM);
    assert(strcmp(NEW_ENUM_to_string(MEMBER_1), "MEMBER_1") == 0);

    // Safe Array Access (if map exists)
    // assert(ENUM_SAFE_ARRAY_ACCESS(NEW_ENUM_score, NEW_ENUM, MEMBER_1) == 0);
}
```
