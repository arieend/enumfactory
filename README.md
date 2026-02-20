# EnumFactory: Advanced Enum Generation Macros

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**EnumFactory** is a header-only library (written in C) that provides a powerful and flexible system for generating enums with advanced features, all through the use of preprocessor macros. It automates many of the tedious tasks associated with enums, making your code cleaner, safer, and more maintainable while imposing **zero runtime overhead** for bounds-checking or stringify operations.

## Key Features

- **Automatic String Conversion:** Easily convert enum members to their string representations without manual `switch` statements.
- **Value Mapping:** Create custom maps that associate enum members with specific values (strings or integers).
- **Type-Safe Access:** Access enum-related data in a safe manner, returning `NULL` or neutral elements on out-of-bounds indices.
- **Bounds Checking:** Perform compile-time count evaluations and runtime bounds checks to ensure that enum values are valid.
- **Array Generation:** Automatically generate parallel arrays or lookup switches indexed by enum members.
- **Automatic and Assigned Enum Values:** Allows for automatic enum value assignation (0, 1, 2...) or custom value assignation.

## Getting Started

### Installation

Since **EnumFactory** is a header-only library, there's no need for building or linking. Simply include the `enumfactorymacros.h` header file in your project.

### Basic Usage

1. **Define Your Enum Members:** Create a list macro that lists your enum members via the X-Macro pattern. This single macro acts as the single source of truth.

   ```c
   #define COLOR_ENUM(X, G) \
       X(G, RED) \
       X(G, GREEN) \
       X(G, BLUE)
   ```

2. **Generate the Enum:** Use the `ENUMS_AUTOMATIC` or `ENUMS_ASSIGNED` macro to generate the enum type and associated lookup functions.
   - **Automatic Value Assignment:**

     ```c
     ENUMS_AUTOMATIC(COLOR);
     ```

   - **Assigned Value Assignment:**

     ```c
     #define HTTP_STATUS_ENUM(X, G) \
         X(G, OK, 200) \
         X(G, NOT_FOUND, 404) \
         X(G, ERROR, 500)

     ENUMS_ASSIGNED(HTTP_STATUS);
     ```

3. **Using the Enum:** Access the generated enum, strings, and safety features.

   ```c
   #include "enumfactorymacros.h"
   #include <stdio.h>

   // 1. Definition
   #define COLOR_ENUM(X, G) \
       X(G, RED) \
       X(G, GREEN) \
       X(G, BLUE)

   // 2. Generation
   ENUMS_AUTOMATIC(COLOR);
   ENUM_TO_STRING(COLOR); // Optional wrapper to generate `COLOR_to_string`

   int main() {
       // Accessing the string representations
       printf("GREEN as string: %s\n", COLOR_get_label(GREEN)); // Output: GREEN

       // Bounds checking
       printf("Is RED a valid color? %d\n", ENUM_IS_VALID(COLOR, RED)); // Output: 1
       printf("Is 99 a valid color? %d\n", ENUM_IS_VALID(COLOR, 99));   // Output: 0

       // Safe array access for parallel structures
       const char* my_colors[] = { "Red Paint", "Green Paint", "Blue Paint" };
       const char* safe_val = ENUM_SAFE_ARRAY_ACCESS(my_colors, COLOR, 99);
       if (!safe_val) {
           printf("Caught out-of-bounds access safely!\n");
       }

       // Count operations (compile-time calculated)
       printf("Total defined colors: %d\n", ENUM_COUNT(COLOR)); // Output: 3

       return 0;
   }
   ```

## Available Macros

### Generation APIs

- `ENUMS_AUTOMATIC(_enum_name)`: Generates an enum with automatic values (0, 1, 2, ...) and an inline function `_enum_name_get_label(int)` leveraging jump tables.
- `ENUMS_ASSIGNED(_enum_name)`: Generates an enum with explicit compiler-assigned values.
- `ENUMS_ARRAY(_enum_name, _enum_list, _type, _suffix)`: Generates an arbitrary parallel mapping (e.g. enum -> score integers).
- `ENUMS_MAP(_enum_name, _enum_list, _generator, _type, _suffix)`: Generates both the enum and its associated parallel mapping in one shot.

### Constants Generated

Any generation API produces the following constants for `PREFIX_`:

- `PREFIX_total`: Range bounding marker (highest value + 1). Use for array allocations.
- `PREFIX_count`: Actual compiled number of members defined. Use for logic needing the element counts.

### Utility Macros

- `ENUM_IS_VALID(_enum, _value)`: Checks if a value strictly matches a defined enum member. Returns a boolean representation.
- `ENUM_SAFE_ARRAY_ACCESS(_array, _enum, _index)`: Provides safe access to a custom user array, returning `(void*)0` (NULL) if out of bounds.
- `ENUM_TO_STRING(_enum)`: Wrapper macro to expose an `_to_string()` alias over the generated `_get_label()` function.

## Best Practices: Documentation

When using these macros, it is recommended to add a comment block explaining the generated values, as X-macro expansions can be hard to track for IDEs.

Example:

```c
ENUMS_ASSIGNED(STATUS);
/*
 * Generated Enum: STATUS
 * -------------------------
 * Type: Assigned
 * Actual Member Count: 3
 * Range (total): 0 to 501 (exclusive)
 *
 * Members & Values:
 * - OK = 200
 * - NOT_FOUND = 404
 * - ERROR = 500
 */
```

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This library is distributed under the MIT License. See `LICENSE` for more information.
