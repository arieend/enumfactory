#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "enumfactorymacros.h"

// Test enum with automatic values
#define COLOR_ENUM(GENERATOR) \
    GENERATOR(RED), \
    GENERATOR(GREEN), \
    GENERATOR(BLUE)

ENUMS_AUTOMATIC(COLOR)

// Test enum with assigned values
#define STATUS_ENUM(GENERATOR) \
    GENERATOR(OK, 200), \
    GENERATOR(NOT_FOUND, 404), \
    GENERATOR(ERROR, 500)

ENUMS_ASSIGNED(STATUS)

// Test cases
void test_automatic_enum(void) {
    assert(RED == 0);
    assert(GREEN == 1);
    assert(BLUE == 2);
    assert(COLOR_total == 3);
    
    assert(strcmp(COLOR_label[RED], "RED") == 0);
    assert(strcmp(COLOR_label[GREEN], "GREEN") == 0);
    assert(strcmp(COLOR_label[BLUE], "BLUE") == 0);
}

void test_assigned_enum(void) {
    assert(OK == 200);
    assert(NOT_FOUND == 404);
    assert(ERROR == 500);
    assert(STATUS_total == 3);
    
    assert(strcmp(STATUS_label[OK], "OK") == 0);
    assert(strcmp(STATUS_label[NOT_FOUND], "NOT_FOUND") == 0);
    assert(strcmp(STATUS_label[ERROR], "ERROR") == 0);
}

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
