/*
 * EnumFactory Test Suite
 * =====================
 * This test suite validates all functionality provided by the enumfactory macros:
 * 1. Automatic enum generation
 * 2. Value-assigned enum generation
 * 3. String label mapping
 * 4. Safety features
 * 5. Bounds checking
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "enumfactorymacros.h"

/*-----------------------------------------------------------------------------
 * Test Enum Definitions
 *-----------------------------------------------------------------------------*/

/* Define a basic color enum for testing automatic value assignment
 * Expected values: RED=0, GREEN=1, BLUE=2 
 *
 * HOW IT WORKS:
 * The X-macro pattern is used here. X is the generator, G is the context.
 * When expanded by ENUMS_AUTOMATIC(COLOR):
 * - X becomes ENUM (invokes ENUM(G, RED), etc.)
 * - G becomes the generator argument (e.g., X_COMMA_)
 * result: RED, GREEN, BLUE,
 */
#define COLOR_ENUM(X, G) \
    X(G, RED) \
    X(G, GREEN) \
    X(G, BLUE)

ENUMS_AUTOMATIC(COLOR);  // Add semicolon

/* Define an HTTP status enum for testing manual value assignment
 * Demonstrates real-world enum usage with specific values 
 *
 * HOW IT WORKS:
 * Here, we provide 3 arguments to X.
 * When expanded by ENUMS_ASSIGNED(STATUS):
 * - X becomes ENUM_VALUE_ASSIGN
 * - It generates: OK = 200, NOT_FOUND = 404, ERROR = 500,
 */
#define STATUS_ENUM(X, G) \
    X(G, OK, 200) \
    X(G, NOT_FOUND, 404) \
    X(G, ERROR, 500)

ENUMS_ASSIGNED(STATUS);
/*
 * Generated Enum: STATUS
 * -------------------------
 * Type: Assigned
 * Actual Member Count: 3
 * Range (total): 0 to 501 (exclusive)
 *
 * Members & Values:
 * - OK = 200 ("OK")
 * - NOT_FOUND = 404 ("NOT_FOUND")
 * - ERROR = 500 ("ERROR")
 */

/* Define a Fruit enum to check for symbol collisions */
#define FRUIT_ENUM(X, G) \
    X(G, APPLE) \
    X(G, ORANGE) \
    X(G, BANANA)

ENUMS_AUTOMATIC(FRUIT);
/*
 * Generated Enum: FRUIT
 * -------------------------
 * Type: Automatic
 * Actual Member Count: 3
 * Range (total): 0 to 3 (exclusive)
 *
 * Members & Values:
 * - APPLE = 0 ("APPLE")
 * - ORANGE = 1 ("ORANGE")
 * - BANANA = 2 ("BANANA")
 */

/* Define a Priority enum with a custom score map */
#define PRIORITY_ENUM(X, G) \
    X(G, LOW, 1) \
    X(G, MEDIUM, 5) \
    X(G, HIGH, 10)

/* Parallel Data Generation (Multi-Attribute)
 * This macro allows us to generate multiple data arrays that align with the Priority enum.
 * By reusing the X-macro structure, different fields are extracted using index parameters.
 */
#define PRIORITY_SCORE_GEN(X, G) \
    X(G, LOW, 0, 0.1f, "just", 3300) \
    X(G, MEDIUM, 50, 0.11f, "trest", 2000) \
    X(G, HIGH, 100, 0.9f, "test1", 1000)

ENUMS_ASSIGNED(PRIORITY);
ENUMS_ARRAY(PRIORITY, PRIORITY_SCORE_GEN, int, score);
ENUMS_ARRAY(PRIORITY, PRIORITY_SCORE_GEN, float, rate, 1);
ENUMS_ARRAY(PRIORITY, PRIORITY_SCORE_GEN, const char*, word, 2);
ENUMS_ARRAY(PRIORITY, PRIORITY_SCORE_GEN, int, amount, 3);
/*
 * Generated Enum: PRIORITY
 * -------------------------
 * Type: Map (Enum and Multi-Attribute Arrays)
 * Actual Member Count: 3
 * Range (total): 0 to 11 (exclusive)
 *
 * Mappings:
 * - LOW = 1 (Score: 0, Rate: 0.1, Word: "just", Amount: 3300)
 * - MEDIUM = 5 (Score: 50, Rate: 0.11, Word: "trest", Amount: 2000)
 * - HIGH = 10 (Score: 100, Rate: 0.9, Word: "test1", Amount: 1000)
 */

#define STATUS_DESC_GEN(X, G) \
    X(G, OK, "All systems go") \
    X(G, NOT_FOUND, "Resource missing") \
    X(G, ERROR, "Internal failure")

ENUMS_ARRAY(STATUS, STATUS_DESC_GEN, const char*, description);

ENUM_TO_STRING(COLOR);
ENUM_TO_STRING(FRUIT);
ENUM_TO_STRING(STATUS);
ENUM_TO_STRING(PRIORITY);

/* Test ENUMS_MAP which generates both enum and array at the same time */
#define EVENT_ENUM(X, G) \
    X(G, CLICK, 100) \
    X(G, HOVER, 200) \
    X(G, SCROLL, 300)

ENUMS_MAP(EVENT, EVENT_ENUM, ENUM, int, code);
ENUM_TO_STRING(EVENT);

/* COLOR_HEX_DATA: associates hex color strings with COLOR enum members.
 * Used to exercise ENUM_VALUE_MAP, ENUM_VALUE_MAP_VAL_, and ENUM_SWITCH_CASE_VAL_.
 *
 * Mappings: RED->"#FF0000", GREEN->"#00FF00", BLUE->"#0000FF"
 */
#define COLOR_HEX_DATA(X, G) \
    X(G, RED,   "#FF0000") \
    X(G, GREEN, "#00FF00") \
    X(G, BLUE,  "#0000FF")

/* Exercises ENUM_VALUE_MAP + ENUM_VALUE_MAP_VAL_:
 * builds a designated-initializer array mapping enum values to strings. */
static const char* color_hex[COLOR_total] = {
    COLOR_HEX_DATA(X_COMMA_, ENUM_VALUE_MAP)
};

/* Exercises ENUM_SWITCH_CASE_VAL_ (non-indexed variant):
 * builds a switch-based getter equivalent to an ENUMS_ARRAY at index 0. */
static inline const char* color_hex_get(int value) {
    switch (value) {
        COLOR_HEX_DATA(X_CALL_, ENUM_SWITCH_CASE_VAL_)
        default: return NULL;
    }
}

/*
 * Generated Array: STATUS_description
 * -------------------------
 * Type: String Value Map
 *
 * Mappings:
 * - OK (200) -> "All systems go"
 * - NOT_FOUND (404) -> "Resource missing"
 * - ERROR (500) -> "Internal failure"
 */

/*-----------------------------------------------------------------------------
 * Test Implementation
 *-----------------------------------------------------------------------------*/

/* Test automatic enum value generation and string mapping
 * Validates:
 * - Sequential value assignment
 * - String label generation
 * - Total count calculation */
void test_automatic_enum(void) {
    assert(RED == 0);
    assert(GREEN == 1);
    assert(BLUE == 2);
    assert(COLOR_total == 3);
    assert(ENUM_COUNT(COLOR) == 3);

    assert(strcmp(COLOR_get_label(RED), "RED") == 0);
    assert(strcmp(COLOR_get_label(GREEN), "GREEN") == 0);
    assert(strcmp(COLOR_get_label(BLUE), "BLUE") == 0);

    assert(COLOR_get_label(99) == NULL);
}

/* Test manually assigned enum values and string mapping
 * Validates:
 * - Custom value assignment
 * - String label generation
 * - Total count calculation */
void test_assigned_enum(void) {
    assert(OK == 200);
    assert(NOT_FOUND == 404);
    assert(ERROR == 500);
    assert(STATUS_total == 501); // Highest value (500) + 1
    assert(ENUM_COUNT(STATUS) == 3);

    assert(strcmp(STATUS_get_label(OK), "OK") == 0);
    assert(strcmp(STATUS_get_label(NOT_FOUND), "NOT_FOUND") == 0);
    assert(strcmp(STATUS_get_label(ERROR), "ERROR") == 0);
}

/* Test multi-enum collision and basic properties */
void test_fruit_enum(void) {
    assert(APPLE == 0);
    assert(BANANA == 2);
    assert(ENUM_COUNT(FRUIT) == 3);
    assert(strcmp(FRUIT_to_string(ORANGE), "ORANGE") == 0);
}

/* Test custom value maps */
void test_priority_map(void) {
    assert(LOW == 1);
    assert(HIGH == 10);
    assert(PRIORITY_total == 11);

    // Test the score map using getter (index 0)
    assert(PRIORITY_get_score(LOW) == 0);
    assert(PRIORITY_get_score(MEDIUM) == 50);
    assert(PRIORITY_get_score(HIGH) == 100);

    // Test the rate map using getter (index 1)
    // Float comparison is tricky, but exact assignment from macro should match
    assert(PRIORITY_get_rate(LOW) == 0.1f);
    assert(PRIORITY_get_rate(MEDIUM) == 0.11f);
    assert(PRIORITY_get_rate(HIGH) == 0.9f);

    // Test the word map using getter (index 2)
    assert(strcmp(PRIORITY_get_word(LOW), "just") == 0);
    assert(strcmp(PRIORITY_get_word(MEDIUM), "trest") == 0);
    assert(strcmp(PRIORITY_get_word(HIGH), "test1") == 0);

    // Test the amount map using getter (index 3)
    assert(PRIORITY_get_amount(LOW) == 3300);
    assert(PRIORITY_get_amount(MEDIUM) == 2000);
    assert(PRIORITY_get_amount(HIGH) == 1000);

    // Test safe access to maps (bounds)
    assert(PRIORITY_get_score(MEDIUM) == 50);
    assert(PRIORITY_get_score(99) == 0);

    assert(PRIORITY_get_rate(99) == 0.0f);
    assert(PRIORITY_get_word(99) == NULL);
    assert(PRIORITY_get_amount(99) == 0);

    assert(PRIORITY_count == 3);
}

/* Test custom string maps */
void test_status_description(void) {
    assert(strcmp(STATUS_get_description(OK), "All systems go") == 0);
    assert(strcmp(STATUS_get_description(NOT_FOUND), "Resource missing") == 0);
    assert(STATUS_get_description(201) == NULL);
}

/* Test ENUMS_MAP macro */
void test_enums_map(void) {
    assert(CLICK == 0);
    assert(HOVER == 1);
    assert(SCROLL == 2);
    assert(EVENT_total == 3);
    assert(ENUM_COUNT(EVENT) == 3);

    assert(EVENT_get_code(CLICK) == 100);
    assert(EVENT_get_code(HOVER) == 200);
    assert(EVENT_get_code(SCROLL) == 300);
    assert(EVENT_get_code(99) == 0);

    assert(strcmp(EVENT_get_label(CLICK), "CLICK") == 0);
}



/* Test safety features and bounds checking
 * Validates:
 * - Valid value detection
 * - Invalid value detection
 * - Array bounds protection
 * - NULL returns for invalid access */
void test_enum_safety(void) {
    // Test valid access
    assert(ENUM_IS_VALID(COLOR, RED));
    assert(ENUM_IS_VALID(COLOR, GREEN));
    assert(ENUM_IS_VALID(COLOR, BLUE));

    // Test invalid access
    assert(!ENUM_IS_VALID(COLOR, -1));
    assert(!ENUM_IS_VALID(COLOR, COLOR_total));

    // Let's create a local array to test ENUM_SAFE_ARRAY_ACCESS
    const char* test_arr[4] = { "r", "g", "b", "null" };
    assert(ENUM_SAFE_ARRAY_ACCESS(test_arr, COLOR, RED) != NULL);
    assert(ENUM_SAFE_ARRAY_ACCESS(test_arr, COLOR, -1) == NULL);
    assert(ENUM_SAFE_ARRAY_ACCESS(test_arr, COLOR, COLOR_total) == NULL);

    // Test sparse enum validity
    assert(ENUM_IS_VALID(STATUS, OK));
    assert(!ENUM_IS_VALID(STATUS, 201)); // Gap between 200 and 404
}

void test_enum_to_string(void) {
    assert(strcmp(COLOR_to_string(RED), "RED") == 0);
    assert(strcmp(FRUIT_to_string(APPLE), "APPLE") == 0);
    assert(strcmp(STATUS_to_string(ERROR), "ERROR") == 0);
}

void print_fruit_values(void) {
    printf("FRUIT values:\n");
    for (int i = 0; i < FRUIT_total; i++) {
        const char* name = FRUIT_to_string(i);
        if (name) {
             printf("  %s = %d\n", name, i);
        }
    }
}

void print_color_values(void) {
    printf("COLOR values:\n");
    for (int i = 0; i < COLOR_total; i++) {
        const char* name = COLOR_to_string(i);
        if (name) {
             printf("  %s = %d\n", name, i);
        }
    }
}

void print_status_values(void) {
    printf("STATUS values:\n");
#define PRINT_STATUS_(name, val, ...) printf("  " #name " = %d\n", val);
    STATUS_ENUM(X_CALL_, PRINT_STATUS_)
#undef PRINT_STATUS_
}

void print_priority_values(void) {
    printf("PRIORITY values:\n");
    for (int i = 0; i < PRIORITY_total; i++) {
        const char* name = PRIORITY_to_string(i);
        if (name) {
             printf("  %s = %d\n", name, i);
        }
    }
}

/* Test ENUM_VALUE_MAP, ENUM_VALUE_MAP_VAL_, and ENUM_SWITCH_CASE_VAL_ */
void test_enum_value_map(void) {
    assert(strcmp(color_hex[RED],   "#FF0000") == 0);
    assert(strcmp(color_hex[GREEN], "#00FF00") == 0);
    assert(strcmp(color_hex[BLUE],  "#0000FF") == 0);

    assert(strcmp(color_hex_get(RED),   "#FF0000") == 0);
    assert(strcmp(color_hex_get(GREEN), "#00FF00") == 0);
    assert(strcmp(color_hex_get(BLUE),  "#0000FF") == 0);
    assert(color_hex_get(99) == NULL);
}

/* Test ENUM_BEGIN and ENUM_END */
void test_enum_range(void) {
    assert(ENUM_BEGIN(COLOR) == 0);
    assert(ENUM_END(COLOR) == COLOR_total);

    int count = 0;
    for (int i = ENUM_BEGIN(COLOR); i < ENUM_END(COLOR); i++) {
        if (ENUM_IS_VALID(COLOR, i)) count++;
    }
    assert(count == ENUM_COUNT(COLOR));
}

/* Main test runner
 * Executes all test cases and reports results */
int main(void) {
    printf("Running enum factory tests...\n");

    test_automatic_enum();
    printf("Automatic enum tests passed\n");

    test_assigned_enum();
    printf("Assigned enum tests passed\n");

    test_enum_safety();
    printf("Safety tests passed\n");

    test_enum_to_string();
    printf("String conversion tests passed\n");

    test_fruit_enum();
    printf("Multi-enum collision tests passed\n");

    test_priority_map();
    printf("Custom map tests passed\n");

    test_status_description();
    printf("Status description tests passed\n");

    test_enums_map();
    printf("ENUMS_MAP tests passed\n");

    test_enum_value_map();
    printf("ENUM_VALUE_MAP and ENUM_SWITCH_CASE_VAL_ tests passed\n");

    test_enum_range();
    printf("ENUM_BEGIN/ENUM_END range tests passed\n");

    print_fruit_values();
    print_color_values();
    print_status_values();
    print_priority_values();

    printf("All tests passed successfully!\n");
    return 0;
}
