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
 * Expected values: RED=0, GREEN=1, BLUE=2 */
#define COLOR_ENUM(X, G) \
    X(G, RED) \
    X(G, GREEN) \
    X(G, BLUE)

ENUMS_AUTOMATIC(COLOR);  // Add semicolon

/* Define an HTTP status enum for testing manual value assignment
 * Demonstrates real-world enum usage with specific values */
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

#define PRIORITY_SCORE_GEN(X, G) \
    X(G, LOW, 0) \
    X(G, MEDIUM, 50) \
    X(G, HIGH, 100)

ENUMS_ASSIGNED(PRIORITY);
ENUMS_ARRAY(PRIORITY, PRIORITY_SCORE_GEN, int, score);
/*
 * Generated Enum: PRIORITY
 * -------------------------
 * Type: Map (Enum and Score Array)
 * Actual Member Count: 3
 * Range (total): 0 to 11 (exclusive)
 *
 * Members & Values:
 * - LOW = 1 (Score: 0)
 * - MEDIUM = 5 (Score: 50)
 * - HIGH = 10 (Score: 100)
 */

#define STATUS_DESC_GEN(X, G) \
    X(G, OK, "All systems go") \
    X(G, NOT_FOUND, "Resource missing") \
    X(G, ERROR, "Internal failure")

ENUMS_ARRAY(STATUS, STATUS_DESC_GEN, const char*, description);
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

    assert(strcmp(COLOR_label[RED], "RED") == 0);
    assert(strcmp(COLOR_label[GREEN], "GREEN") == 0);
    assert(strcmp(COLOR_label[BLUE], "BLUE") == 0);

    assert(strcmp(COLOR_get_label(RED), "RED") == 0);
    assert(strcmp(COLOR_get_label(99), "") == 0 || COLOR_get_label(99) == NULL);
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

    assert(strcmp(STATUS_label[OK], "OK") == 0);
    assert(strcmp(STATUS_label[NOT_FOUND], "NOT_FOUND") == 0);
    assert(strcmp(STATUS_label[ERROR], "ERROR") == 0);
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

    // Test the score map
    assert(PRIORITY_score[LOW] == 0);
    assert(PRIORITY_score[MEDIUM] == 50);
    assert(PRIORITY_score[HIGH] == 100);

    // Test safe access to score map
    assert(PRIORITY_get_score(MEDIUM) == 50);
    assert(PRIORITY_get_score(99) == 0);
}

/* Test custom string maps */
void test_status_description(void) {
    assert(strcmp(STATUS_description[OK], "All systems go") == 0);
    assert(strcmp(STATUS_get_description(NOT_FOUND), "Resource missing") == 0);
    assert(STATUS_get_description(201) == NULL);
}

ENUM_TO_STRING(COLOR);
ENUM_TO_STRING(FRUIT);
ENUM_TO_STRING(STATUS);

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

    // Test safe access
    assert(ENUM_SAFE_ARRAY_ACCESS(COLOR_label, COLOR, RED) != NULL);
    assert(ENUM_SAFE_ARRAY_ACCESS(COLOR_label, COLOR, -1) == NULL);
    assert(ENUM_SAFE_ARRAY_ACCESS(COLOR_label, COLOR, COLOR_total) == NULL);

    // Test sparse enum validity
    assert(ENUM_IS_VALID(STATUS, OK));
    assert(!ENUM_IS_VALID(STATUS, 201)); // Gap between 200 and 404
}

void test_enum_to_string(void) {
    assert(strcmp(COLOR_to_string(RED), "RED") == 0);
    assert(strcmp(FRUIT_to_string(APPLE), "APPLE") == 0);
    assert(strcmp(STATUS_to_string(ERROR), "ERROR") == 0);
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

    printf("All tests passed successfully!\n");
    return 0;
}
