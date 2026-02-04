# EnumFactory: Advanced Enum Generation Macros

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**EnumFactory** is a header-only C++ library that provides a powerful and flexible system for generating enums with advanced features, all through the use of preprocessor macros. It automates many of the tedious tasks associated with enums, making your code cleaner, safer, and more maintainable.

## Key Features

- **Automatic String Conversion:** Easily convert enum members to their string representations without manual `switch` statements.
- **Value Mapping:** Create custom maps that associate enum members with specific values (strings or integers).
- **Type-Safe Access:** Access enum-related data in a type-safe manner, reducing the risk of errors.
- **Bounds Checking:** Perform runtime bounds checks to ensure that enum values are valid, preventing unexpected behavior.
- **Array Generation:** Automatically generate arrays indexed by enum members, storing string labels or custom values.
- **Automatic and Assigned Enum Values:** Allows for automatic enum value assignation (0,1,2,...) or custom value assignation.

## Getting Started

### Installation

Since **EnumFactory** is a header-only library, there's no need for building or linking. Simply include the `enumfactorymacros.h` header file in your project.

### Basic Usage

1.  **Define Your Enum Members:** Create a macro that lists your enum members. This macro must take a `GENERATOR` argument.

    ```cpp
    #define COLOR_ENUM(X, G) \
        X(G, RED) \
        X(G, GREEN) \
        X(G, BLUE)
    ```

2.  **Generate the Enum:** Use the `ENUMS_AUTOMATIC` or `ENUMS_ASSIGNED` macro to generate the enum type and associated structures.
    - **Automatic Value Assignment:**

      ```cpp
      ENUMS_AUTOMATIC(COLOR)
      ```

      This generates:

      ```cpp
      typedef enum {
         RED, //0
         GREEN, //1
         BLUE, //2
         COLOR_total
      } COLOR;

      static const char* COLOR_label[COLOR_total] = {
          [RED] = "RED",
          [GREEN] = "GREEN",
          [BLUE] = "BLUE",
      };
      ```

    - **Assigned Value Assignment:**

      ```cpp
       #define COLOR_ENUM(GENERATOR) \
          GENERATOR(RED,10), \
          GENERATOR(GREEN,20), \
          GENERATOR(BLUE,30)
      ```

      ```cpp
      ENUMS_ASSIGNED(COLOR)
      ```

      This generates:

      ```cpp
      typedef enum {
          RED = 10,
          GREEN = 20,
          BLUE = 30,
          COLOR_total
      } COLOR;

      static const char* COLOR_label[COLOR_total] = {
          [RED] = "RED",
          [GREEN] = "GREEN",
          [BLUE] = "BLUE",
      };
      ```

3.  **Using the Enum:** Access the generated enum, labels, and safety features.

    ````cpp
    #include "enumfactorymacros.h"
    #include <iostream>

    ```cpp
    #include "enumfactorymacros.h"
    #include <iostream>

    #define COLOR_ENUM(X, G) \
        X(G, RED) \
        X(G, GREEN) \
        X(G, BLUE)

    ENUMS_AUTOMATIC(COLOR)

    int main() {
        // Accessing the label array
        std::cout << "RED label: " << COLOR_label[COLOR::RED] << std::endl; // Output: RED label: RED
        std::cout << "GREEN label: " << COLOR_label[COLOR::GREEN] << std::endl; // Output: GREEN label: GREEN

        // Bounds checking
        std::cout << "Is 2 (BLUE) a valid color? " << ENUM_IS_VALID(COLOR, 2) << std::endl; // Output: Is 2 (BLUE) a valid color? 1
        std::cout << "Is -1 valid color? " << ENUM_IS_VALID(COLOR, -1) << std::endl; // Output: Is -1 a valid color? 0

        // Safe array access
        const char* label = ENUM_SAFE_ARRAY_ACCESS(COLOR_label, COLOR, COLOR::RED);
        if (label) {
            std::cout << "Safe access: " << label << std::endl; // Output: Safe access: RED
        }
        const char* label_wrong = ENUM_SAFE_ARRAY_ACCESS(COLOR_label, COLOR, 99);
        if (label_wrong) {
            std::cout << "Safe access (wrong index): " << label_wrong << std::endl;
        } else {
            std::cout << "Safe access (wrong index): NULL"<< std::endl; // Output: Safe access (wrong index): NULL
        }

        // Enum to string
        ENUM_TO_STRING(COLOR)
        std::cout << "GREEN as string: "<< COLOR_to_string(COLOR::GREEN) << std::endl; // Output: GREEN as string: GREEN

        return 0;
    }
    ````

    ````cpp
    #include "enumfactorymacros.h"
    #include <iostream>

    ```cpp
    #include "enumfactorymacros.h"
    #include <iostream>

    #define COLOR_ENUM(X, G) \
        X(G, RED, 10) \
        X(G, GREEN, 20) \
        X(G, BLUE, 30)

    ENUMS_ASSIGNED(COLOR)

    int main() {
        std::cout << "RED label: " << COLOR_label[COLOR::RED] << std::endl; // Output: RED label: RED
        std::cout << "GREEN label: " << COLOR_label[COLOR::GREEN] << std::endl; // Output: GREEN label: GREEN
        std::cout << "RED value: " << COLOR::RED << std::endl; // Output: RED value: 10
        std::cout << "GREEN value: " << COLOR::GREEN << std::endl; // Output: GREEN value: 20
        std::cout << "Is 20 (invalid index) a valid color? " << ENUM_IS_VALID(COLOR, 20) << std::endl; // Output: Is 20 (invalid index) a valid color? 0
        std::cout << "Is 10 (valid index) a valid color? " << ENUM_IS_VALID(COLOR, 10) << std::endl; // Output: Is 10 (valid index) a valid color? 0
        return 0;
    }
    ````

## Available Macros

### Base Generation Macros

- `ENUM(_, ...)`: Basic enum member generation (no special behavior).
- `ENUM_INDEX(_, ...)`: Creates an array index accessor from an enum member.
- `ENUM_STRING(_, ...)`: Converts an enum member name to a string literal.
- `ENUM_VALUE_ASSIGN(_, _v, ...)`: Assigns a custom value to an enum member.

### Enum Implementation Macros

- `ENUM_VALUE_MAP(_, _v, ...)`: Creates a map that associates enum members with values.
- `ENUM_STRING_VALUE_MAP(_, _v, ...)`: Creates a map that associates enum members with string values.
- `ENUM_STRING_SELF_MAP(_, ...)`: Creates a map that associates enum members with their own names as strings.
- `ENUM_TOTAL(_)`: Defines the range of the enum (highest value + 1). Use for array allocations.
- `ENUM_COUNT(_)`: Defines the actual number of members defined. Use for logic that needs to know how many options exist.
- `_generate_enums(_enum, _generator)`: Generates the enum type using a specified member generator.
- `_generate_enums_array(_enum, T, _suffix, _generator)`: Generates an array of values associated with the enum members.

### Arrays' Generator Macros

- `ENUMS(_enum, _generator)`: Generates the enum type.
- `ENUMS_ARRAY(_enum, _array_type, _array_suffix, _array_generator)`: Generates an array for the enum.
- `ENUMS_MAP(_enum, _enum_generator, _array_type, _array_suffix, _array_generator)`: Generates both the enum and its associated array.
- `ENUMS_BASE(_, _enum_generator)`: Generates enums and a default label array.
- `ENUMS_AUTOMATIC(_)`: Generates an enum with automatic values (0, 1, 2, ...) and a label array.
- `ENUMS_ASSIGNED(_)`: Generates an enum with assigned values and a label array.

### Safety and Validation Macros

- `ENUM_SAFE_ARRAY_ACCESS(_array, _enum, _index)`: Provides safe access to an enum-related array, returning `NULL` if out of bounds.
- `ENUM_BEGIN(_enum)`: Returns the first valid enum value (0).
- `ENUM_END(_enum)`: Returns one past the last valid enum value.
- `ENUM_IS_VALID(_enum, _value)`: Checks if a value is a valid enum member.
- `ENUM_TO_STRING(_enum)`: Generates a function `_enum_to_string` that converts an enum to its string representation.

## Best Practices: Documentation

When using these macros, it is recommended to add a comment block explaining the generated values, as X-macro expansions can be hard to track.

Example:

```c
ENUMS_ASSIGNED(STATUS);
/*
 * Generated Enum: STATUS
 * -------------------------
 * Type: Assigned
 * Actual Member Count: 3
 * Range (total): 0 to 501 (exclusive)
 */
```

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This library is distributed under the MIT License. See `LICENSE` for more information.
