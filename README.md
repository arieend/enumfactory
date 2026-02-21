# EnumFactory: Advanced Enum Generation Macros

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**EnumFactory** is a header-only library (written in C) that provides a powerful and flexible system for generating enumerations coupled with rich metadata (string labels, element counts, parallel arrays, and safe bounds-checking) via C preprocessor metaprogramming.

It automates many of the tedious chores associated with maintaining enums, making your code cleaner natively while imposing **zero runtime overhead** for bounds-checking or stringify operations.

---

## 🚀 Key Features

| Feature                         | Description                                                                                                       |
| ------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| **Automatic String Conversion** | Easily convert enum members to their string representations without manual `switch` statements.                   |
| **Zero Runtime Overhead**       | The generated code natively utilizes compiled Static Jump Tables (`O(1)` operations) for mappings.                |
| **Multi-Attribute Arrays**      | Automatically generate parallel typed arrays extracting columns from a single macro definition (up to 10 values!) |
| **Type-Safe Access**            | Access enum-related data safely, returning `NULL` or neutral elements on out-of-bounds invalid integers.          |
| **Bounds Checking / Ranges**    | Receive compile-time precise count evaluations natively (`sizeof` based) and macro boundaries.                    |
| **Custom Integer Assignments**  | Supports both automatic enum sequencing (0, 1, 2) and custom value binding (`HTTP_OK = 200`).                     |

---

## 🧠 How It Works: The "Single Source of Truth"

EnumFactory leverages the **X-Macro technique**.

Instead of manually maintaining an `enum { ... }` block and a separate `const char* labels[] = { ... }` that inevitably drift out of sync, developers write the definition of an enum once:

```c
#define HTTP_STATUS_ENUM(X, G) \
    X(G, OK, 200)              \
    X(G, NOT_FOUND, 404)       \
    X(G, SERVER_ERROR, 500)
```

The underlying library macros will pass different generator variants (`G`) to this list to extract identifiers and values without repetition.

### Expansion Pipeline Architecture

```text
 +--------------------------+
 |  HTTP_STATUS_ENUM(X, G)  |  <-- Single Source of Truth
 +-------------+------------+
               |
               v
 +-------------+-------------+
 |     GENERATE_ENUM_CORE    |  <-- Injects Extractors (e.g. ENUM_VALUE_ASSIGN)
 +----+--------+--------+----+
      |        |        |
      v        v        v
   enum{}   _count   _get_label(int value)
               ^           ^
   Constant Compile  O(1) Bounds Switch Map
```

---

## 🛠️ Usage Examples

### 1. Installation

As a header-only library, no building or linking steps are required.
Simply copy and include `enumfactorymacros.h` in your project!

### 2. Automatic Array Generation

Use `ENUMS_AUTOMATIC` to generate incrementing index values.

```c
#include "enumfactorymacros.h"
#include <stdio.h>

// 1. Definition
#define COLOR_ENUM(X, G) \
    X(G, RED)   \
    X(G, GREEN) \
    X(G, BLUE)

// 2. Generation Engine
ENUMS_AUTOMATIC(COLOR);

// 3. String wrapper macro
ENUM_TO_STRING(COLOR);

int main() {
    // Zero-overhead string representation
    printf("Color: %s\n", COLOR_get_label(GREEN)); // Outputs "GREEN"

    // Automatic type-safe bounds checking on C integers!
    if (ENUM_IS_VALID(COLOR, 15)) {
        printf("Valid!\n");
    } else {
        printf("Invalid Color (not found).\n"); // Runs successfully.
    }

    // Constant range compilation
    printf("Defined color count: %d\n", ENUM_COUNT(COLOR)); // Outputs 3

    return 0;
}
```

### 3. Multi-Attribute Parallel Maps (`ENUMS_ARRAY`)

Data points often possess multiple pieces of integrated metadata. With recent updates, EnumFactory can map and sequentially extract up to **10 independent attribute arrays** from a single tuple list:

```c
// Define the Single Source of Truth (Macro Name, ID, Price, Class)
#define PRODUCT_ENUM(X, G) \
    X(G, LAPTOP, 1, 999.99f, "Computer") \
    X(G, MOUSE,  2, 49.99f,  "Accessory")

// Generates the enum itself binding values mapped exactly to Index 0
ENUMS_MAP(PRODUCT, PRODUCT_ENUM, ENUM_VALUE_ASSIGN, int, item_id);

// Generate isolated map parallel arrays extracting target index columns:
ENUMS_ARRAY(PRODUCT, PRODUCT_ENUM, float, price, 1);       // Index 1 (Float)
ENUMS_ARRAY(PRODUCT, PRODUCT_ENUM, const char*, class, 2); // Index 2 (String)

int main() {
    printf("Laptop ID: #%d\n", PRODUCT_get_item_id(LAPTOP));
    printf("Laptop Price: $%.2f\n", PRODUCT_get_price(LAPTOP));
    printf("Laptop Class: %s\n", PRODUCT_get_class(LAPTOP));
}
```

See `examples/stock_market_example.c` for an advanced demo mapping 10 market variables natively.

---

## 📖 API Macro Reference

### Generation Core

- `ENUMS_AUTOMATIC(_enum_name)`: Outputs standard enum values (0, 1) and string jump tables.
- `ENUMS_ASSIGNED(_enum_name)`: Outputs explicit compiler assignments bounded to string conversions.
- `ENUMS_ARRAY(_enum_name, _enum_list, _type, _suffix, [index])`: Generates parallel maps. `index` parameter (defaults to 0, up to 10) selects the extraction tuple index.
- `ENUMS_MAP(_enum_name, _enum_list, _generator, _type, _suffix)`: Shorthand to generate both an enum and array map sequentially.

### Generated Endpoints

For any `PREFIX` Enum generated (Substitute `PREFIX` with your enum name):

- `PREFIX_total`: Marker mapping the bounds highest allocated boundary + 1.
- `PREFIX_count`: Safe `const int` tracking explicitly exact member counts.
- `PREFIX_get_label(int)`: Bound-safe return of the designated literal string name.

### Utility Macros

- `ENUM_IS_VALID(_enum, _value)`: Assesses if a raw integer exists functionally within bounds.
- `ENUM_SAFE_ARRAY_ACCESS(_array, _enum, _index)`: Validates bound limits of an independent mapping array.

---

## 🧪 Building & Testing

### Prerequisites

EnumFactory requires a standard C compiler (like GCC or Clang).

**For Windows Users:** If you don't have native MinGW binaries configured, you can seamlessly test and build the project using **WSL (Windows Subsystem for Linux)**.

```bash
# Verify the test suite covers all bounds and mappings
make test

# Clear compilation artifacts
make clean
```

Tests validate sequential counts, string assignments, multi-variable collision boundaries, and custom parameter limits in `test/enumfactory_test.c`.

---

## 🤝 Contributing

We heartily welcome contributions extending generator limitations, reporting undefined expansion bounds, or enhancing compilation documentation. Please open issues or submit branch pull requests.

## 📄 License

This library is fully distributed under the **MIT License**.
See `LICENSE` for more information.
