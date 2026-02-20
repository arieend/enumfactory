# Enum Factory Macros Engine in C

## 1. Introduction

The Enum Factory Macros Engine is an advanced C preprocessor metaprogramming system to generate enumerations coupled with rich metadata (string labels, element counts, parallel arrays, and safe bounds-checking).

By leveraging the **X-Macro** technique, developers write the definition of an enum once (the single source of truth). From that single definition, the compiler deterministically generates the enum constants, lookup functions, stringification mappings, and strict count thresholds.

---

## 2. X-Macro Technique: The Single Source of Truth

The core pattern revolves around defining a "List Macro" that defers execution to a visitor macro (`X`) and a generator macro (`G`).

```c
// The Single Source of Truth
#define HTTP_STATUS_ENUM(X, G) \
    X(G, OK, 200)              \
    X(G, NOT_FOUND, 404)       \
    X(G, SERVER_ERROR, 500)
```

Instead of manually maintaining an `enum { ... }` and a separate `const char* labels[] = { ... }` that usually drift out of sync, the `HTTP_STATUS_ENUM` encodes structural knowledge. The meta-macros will pass different variants of `X` and `G` to this list to extract identifiers and values without repetition.

---

## 3. Step-by-Step Macro Substitution

Let's examine how the `#define ENUMS_ASSIGNED(HTTP_STATUS)` generates code.

### Phase 1: Input Macro Expansion

The call to `ENUMS_ASSIGNED(HTTP_STATUS)` resolves to:

```c
GENERATE_ENUM_CORE(HTTP_STATUS, HTTP_STATUS_ENUM, ENUM_VALUE_ASSIGN)
```

### Phase 2: Generating the Enum Block

Inside `GENERATE_ENUM_CORE`, the list is evaluated to create the C `enum`:

```c
typedef enum {
    // Evaluation: HTTP_STATUS_ENUM(_X_COMMA, ENUM_VALUE_ASSIGN)
    _X_COMMA(ENUM_VALUE_ASSIGN, OK, 200)
    _X_COMMA(ENUM_VALUE_ASSIGN, NOT_FOUND, 404)
    _X_COMMA(ENUM_VALUE_ASSIGN, SERVER_ERROR, 500)
    HTTP_STATUS_total
} HTTP_STATUS;
```

### Phase 3: Generator Expansion

`_X_COMMA` appends the trailing comma, and `ENUM_VALUE_ASSIGN` maps `A = B`.

```c
typedef enum {
    OK = 200,          // From ENUM_VALUE_ASSIGN(OK, 200) + _X_COMMA
    NOT_FOUND = 404,
    SERVER_ERROR = 500,
    HTTP_STATUS_total  // Marker
} HTTP_STATUS;
```

### Phase 4: Size Calculation (The Array Trick)

The exact count is instantiated via compile-time evaluation. `_ENUM_VAL_COUNT` returns `1,` for each substitution:

```c
static const int HTTP_STATUS_count = (sizeof((int[]){ 1, 1, 1, 0 }) / sizeof(int)) - 1; // Evaluates strictly to 3
```

### Phase 5: Generating the Lookup Switch

In the same core macro, a bounds-safe accessor is defined via `_X_CALL`:

```c
static inline const char* HTTP_STATUS_get_label(int value) {
    switch(value) {
        // HTTP_STATUS_ENUM(_X_CALL, ENUM_SWITCH_CASE_LABEL_)
        case OK: return "OK";
        case NOT_FOUND: return "NOT_FOUND";
        case SERVER_ERROR: return "SERVER_ERROR";
        default: return (const char*)0;
    }
}
```

---

## 4. Macro Expansion Flow Diagrams

### Expansion Pipeline Architecture

```text
 +--------------------------+
 |  HTTP_STATUS_ENUM(X, G)  |  <-- Single Source of Truth
 +-------------+------------+
               |
               v
 +-------------+-------------+
 |     GENERATE_ENUM_CORE    |  <-- Injects X (_X_COMMA, _X_CALL, _ENUM_VAL_COUNT)
 +----+--------+--------+----+      and G (ENUM, ENUM_VALUE_ASSIGN, ...)
      |        |        |
      v        v        v
   enum{}   _count   _get_label(int value)
```

### Parallel Array Maps Flow (`ENUMS_MAP`)

````text
                   +-------------------+
                   |     ENUMS_MAP     |
                   +--------+----------+
                            |
           +----------------+----------------+
           |                                 |
           v                                 v
   GENERATE_ENUM_CORE                   ENUMS_ARRAY (index 0)
    (Base Enum & Labels)           (Parallel Lookup Array)
           |                                 |
           v                                 v
   enum { ... }                   static inline _type _get_value()

### Multi-Attribute Maps Flow (ENUMS_ARRAY with indices)

Multiple `ENUMS_ARRAY` calls can target the same list with different indices:

```text
               +----------------------------------+
               |      complex_ENUM(X, G)          |
               +----------------------------------+
                 /              |               \
                v               v                v
     ENUMS_ARRAY(..., 0)  ENUMS_ARRAY(..., 1) ENUMS_ARRAY(..., 2)
       (_get_score)         (_get_rate)         (_get_name)
````

````

---

## 5. Compile-Time Behavior vs Runtime Dispatch

- **Compile-Time Execution**:
  The preprocessor determines the structural boundaries (values and string literals). The count generator securely relies on the C99 compound literature `sizeof((int[]){1,1,0})` rule to determine the size of instantiated elements natively without iterations.

- **Runtime Dispatch Strategy**:
  `_get_label` and `_get_<suffix>` are inherently mapped via `switch-case` blocks wrapped in `static inline` functions. This prevents dynamic loop bottlenecks. Supported C compilers uniformly compile sequential switch cases down to fast **O(1) Branch Tables (Jump Tables)**. Consequently, dispatch mappings are computationally exact with no caching or searching overhead.

---

## 6. Minimal Working C Example

```c
#include <stdio.h>
#include "enumfactorymacros.h"

// 1. Define the List
#define COLOR_ENUM(X, G) \
    X(G, RED)            \
    X(G, GREEN)          \
    X(G, BLUE)

// 2. Expand into Auto-Indexed Enum
ENUMS_AUTOMATIC(COLOR);

int main() {
    COLOR my_color = GREEN;

    // 3. Automated capabilities
    printf("Color Enum Count: %d\n", COLOR_count);               // Output: 3
    printf("Green String: %s\n", COLOR_get_label(my_color));     // Output: "GREEN"

    // 4. Checking validity
    if (ENUM_IS_VALID(COLOR, 15)) {
        printf("Valid!\n");
    } else {
        printf("Invalid Color (not found).\n");                  // This runs.
    }

    return 0;
}
````

---

---

## 7. Parallel Map Generation (ENUMS_MAP) Example

To simultaneously generate an enum and a lookup function mapping each enum value to another type (like an `int` code or `const char*` description), use the `ENUMS_MAP` generator:

```c
#include <stdio.h>
#include "enumfactorymacros.h"

// 1. Define the enum and its corresponding mapped values
#define EVENT_ENUM(X, G) \
    X(G, CLICK, 100)     \
    X(G, HOVER, 200)

// 2. Generate the ENUM, count, strings, AND a int-returning _get_code(int value) function
ENUMS_MAP(EVENT, EVENT_ENUM, ENUM, int, code);

int main() {
    printf("Hover event code: %d\n", EVENT_get_code(HOVER)); // Outputs 200
    printf("Unknown event code: %d\n", EVENT_get_code(999)); // Safe fallback, outputs 0
    return 0;
}
```

---

## 8. Multi-Attribute Parallel Map Generation

Data points often possess multiple pieces of metadata. The generator macro can define arbitrary tuple lengths, and `ENUMS_ARRAY` can meticulously extract specific columns using the optional 5th parameter (the index, starting from 0):

```c
#include <stdio.h>
#include "enumfactorymacros.h"

#define PRODUCT_ENUM(X, G) \
    X(G, LAPTOP, 1, 999.99f, "Computer") \
    X(G, MOUSE,  2, 49.99f,  "Accessory")

// Base enum and basic code mapping (index 0)
ENUMS_MAP(PRODUCT, PRODUCT_ENUM, ENUM_VALUE_ASSIGN, int, code);

// Explicit extraction mappings
ENUMS_ARRAY(PRODUCT, PRODUCT_ENUM, float, price, 1);       // Index 1
ENUMS_ARRAY(PRODUCT, PRODUCT_ENUM, const char*, type, 2);  // Index 2

int main() {
    printf("Laptop price: %f\n", PRODUCT_get_price(LAPTOP));
    printf("Laptop type: %s\n", PRODUCT_get_type(LAPTOP));
    return 0;
}
```

## 9. Explanation of Type-Safety Limits in C

C enums possess remarkably weak typing—falling back into arbitrary integers natively (`int` representations).

```c
COLOR my_color = 999; // C compiler accepts this seamlessly.
```

The Macro Factory is purposefully designed to accommodate integer pollution while shielding structural integrity. Because the bounds functions demand precise numerical matches, accessing an array element out-of-parameter via `ENUM_SAFE_ARRAY_ACCESS` or `ENUM_IS_VALID` behaves deterministically by rejecting rogue values. The X-macro wrapper applies pseudo-strict bounds testing despite language-level omissions.

## 10. Error Handling Strategy

The strategy targets complete fault tolerance with unsafe/corrupted enum values:

1. **Implicit Invalid Enums**: The macro structure inherently routes rogue `int`/enum values to the `default` cases of the generated runtime maps.
2. **Deterministic NULL Returns**: Erroneous mappings deliberately dump `(const char*)0` identically to `NULL`.
3. **Macro Guards**: By channeling accesses through `ENUM_IS_VALID(COLOR, value)` locally, programmers verify existence explicitly.
4. **Out-of-Bound Defaults:** Map array implementations ensure bad enum requests fall safely into fallback defaults `return (_type)0;` protecting against classical segmentation faults common in static parallel arrays architectures.
