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
#define COLOR_ENUM(GENERATOR) \
    GENERATOR(RED), \
    GENERATOR(GREEN), \
    GENERATOR(BLUE)

ENUMS_AUTOMATIC(COLOR)

/* Define an HTTP status enum for testing manual value assignment
 * Demonstrates real-world enum usage with specific values */
#define STATUS_ENUM(GENERATOR) \
    GENERATOR(OK, 200), \
    GENERATOR(NOT_FOUND, 404), \
    GENERATOR(ERROR, 500)

ENUMS_ASSIGNED(STATUS)

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
    
    assert(strcmp(COLOR_label[RED], "RED") == 0);
    assert(strcmp(COLOR_label[GREEN], "GREEN") == 0);
    assert(strcmp(COLOR_label[BLUE], "BLUE") == 0);
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
    assert(STATUS_total == 3);
    
    assert(strcmp(STATUS_label[OK], "OK") == 0);
    assert(strcmp(STATUS_label[NOT_FOUND], "NOT_FOUND") == 0);
    assert(strcmp(STATUS_label[ERROR], "ERROR") == 0);
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
    
    // Test safe access
    assert(ENUM_SAFE_ARRAY_ACCESS(COLOR_label, COLOR, RED) != NULL);
    assert(ENUM_SAFE_ARRAY_ACCESS(COLOR_label, COLOR, -1) == NULL);
    assert(ENUM_SAFE_ARRAY_ACCESS(COLOR_label, COLOR, COLOR_total) == NULL);
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
    
    printf("All tests passed successfully!\n");
    return 0;
}
