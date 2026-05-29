# EnumFactory

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

EnumFactory is a header-only C library that generates enumerations with automatic string labels, member counts, parallel data arrays, and bounds-checking — all from a single macro definition, with zero runtime overhead.

---

## The Problem It Solves

Every C project that uses enums eventually needs the same boilerplate: a string array to print names, a count constant, bounds-checking before casting, and maybe parallel data tables that map each member to a price, a code, a description. These artifacts are written by hand and drift out of sync the moment someone reorders or adds a member.

EnumFactory eliminates all of it. You write the definition once; the preprocessor generates everything else.

---

## Quick Start

```c
#include "enumfactorymacros.h"
#include <stdio.h>

#define COLOR_ENUM(X, G) \
    X(G, RED)            \
    X(G, GREEN)          \
    X(G, BLUE)

ENUMS_AUTOMATIC(COLOR);
ENUM_TO_STRING(COLOR);

int main(void) {
    printf("%s\n", COLOR_to_string(GREEN));   // GREEN
    printf("%d\n", ENUM_COUNT(COLOR));        // 3
    printf("%d\n", ENUM_IS_VALID(COLOR, 99)); // 0
    return 0;
}
```

---

## Installation

EnumFactory is a single header. Copy `include/enumfactorymacros.h` into your project or point your compiler at it.

The repo also ships a static library stub so it can be linked as `-lenumfactory`. Build it once with:

```bash
make
```

Then compile your code:

```bash
gcc your_code.c -I./include -L. -lenumfactory -o your_app
```

Run the test suite:

```bash
make test
```

---

## Core Concept: The X-Macro Pattern

EnumFactory uses the **X-Macro technique** — a C preprocessor idiom where a list macro accepts a generator function and applies it to every member in the list.

You define your enum data as a macro that takes two parameters, `X` and `G`:

```c
#define STATUS_ENUM(X, G)  \
    X(G, OK,        200)   \
    X(G, NOT_FOUND, 404)   \
    X(G, ERROR,     500)
```

You never call `STATUS_ENUM` yourself. The library macros (`ENUMS_AUTOMATIC`, `ENUMS_ASSIGNED`, etc.) invoke it internally, passing different generators to extract enum identifiers, assigned values, string labels, and data arrays — all from the same list.

**The key benefit:** there is one source of truth. Add, remove, or reorder a member in one place and every generated artifact updates automatically.

### Naming Convention

Your list macro must be named `<NAME>_ENUM` when using `ENUMS_AUTOMATIC` or `ENUMS_ASSIGNED`, because those macros look for `<NAME>_ENUM` by convention. For `ENUMS_ARRAY` and `ENUMS_MAP`, you pass the list macro explicitly.

```c
#define MYNAME_ENUM(X, G)  ...   // used by ENUMS_AUTOMATIC(MYNAME)
                                  // used by ENUMS_ASSIGNED(MYNAME)

#define ANY_NAME(X, G)     ...   // passed directly to ENUMS_ARRAY / ENUMS_MAP
```

### Member Syntax

Each entry in the list macro follows the form:

```
X(G, MEMBER_NAME [, value [, attr1, attr2, ..., attr10]])
```

- `MEMBER_NAME` — the enum identifier (required)
- `value` — the integer value to assign (required for `ENUMS_ASSIGNED`; ignored for `ENUMS_AUTOMATIC`)
- `attr1..attr10` — additional data columns, accessed by index with `ENUMS_ARRAY`

No trailing commas or semicolons inside the macro body.

---

## Part 1: Automatic Enums — `ENUMS_AUTOMATIC`

Use this when you want sequential values starting at 0, exactly like a plain C enum.

```c
#define DIRECTION_ENUM(X, G) \
    X(G, NORTH)              \
    X(G, SOUTH)              \
    X(G, EAST)               \
    X(G, WEST)

ENUMS_AUTOMATIC(DIRECTION);
```

**What gets generated:**

```c
typedef enum {
    NORTH, SOUTH, EAST, WEST,
    DIRECTION_total         // always the last member; equals member count for automatic enums
} DIRECTION;

static const int DIRECTION_count = 4;

static inline const char* DIRECTION_get_label(int value) {
    switch (value) {
        case NORTH: return "NORTH";
        case SOUTH: return "SOUTH";
        case EAST:  return "EAST";
        case WEST:  return "WEST";
        default:    return NULL;
    }
}
```

**Usage:**

```c
DIRECTION d = NORTH;

printf("%s\n", DIRECTION_get_label(d));        // NORTH
printf("%d\n", ENUM_COUNT(DIRECTION));         // 4
printf("%d\n", ENUM_IS_VALID(DIRECTION, 2));   // 1  (EAST)
printf("%d\n", ENUM_IS_VALID(DIRECTION, 10));  // 0  (out of range)
```

For automatic enums, `DIRECTION_total == DIRECTION_count == 4`. They are equivalent because the values are contiguous from 0.

---

## Part 2: Assigned Enums — `ENUMS_ASSIGNED`

Use this when your enum members need specific integer values — HTTP codes, error codes, protocol constants, etc.

```c
#define HTTP_ENUM(X, G)        \
    X(G, HTTP_OK,        200)  \
    X(G, HTTP_NOT_FOUND, 404)  \
    X(G, HTTP_ERROR,     500)

ENUMS_ASSIGNED(HTTP);
```

**What gets generated:**

```c
typedef enum {
    HTTP_OK = 200, HTTP_NOT_FOUND = 404, HTTP_ERROR = 500,
    HTTP_total      // = 501 (highest value + 1)
} HTTP;

static const int HTTP_count = 3;

static inline const char* HTTP_get_label(int value) {
    switch (value) {
        case 200: return "HTTP_OK";
        case 404: return "HTTP_NOT_FOUND";
        case 500: return "HTTP_ERROR";
        default:  return NULL;
    }
}
```

**Critical distinction: `_count` vs `_total`**

| | Automatic enum | Assigned enum |
|---|---|---|
| `HTTP_count` | number of members | number of members |
| `HTTP_total` | highest value + 1 | highest value + 1 |

For automatic enums these are always the same. For assigned enums they diverge: `HTTP_count == 3` but `HTTP_total == 501`.

Always use `_count` when you need how many members there are. Use `_total` only as a range bound when you know the values are contiguous.

**Sparse enum iteration — do it correctly:**

```c
// WRONG for sparse enums: iterates 501 times, most entries invalid
for (int i = 0; i < HTTP_total; i++) { ... }

// CORRECT: iterate known members directly
const int codes[] = { HTTP_OK, HTTP_NOT_FOUND, HTTP_ERROR };
for (int i = 0; i < HTTP_count; i++) {
    printf("%s = %d\n", HTTP_get_label(codes[i]), codes[i]);
}

// ALSO CORRECT: iterate full range but guard with ENUM_IS_VALID
for (int i = 0; i < HTTP_total; i++) {
    if (ENUM_IS_VALID(HTTP, i))
        printf("%s = %d\n", HTTP_get_label(i), i);
}
```

---

## Part 3: String Conversion — `ENUM_TO_STRING`

`ENUM_get_label(int)` is generated automatically by `ENUMS_AUTOMATIC` / `ENUMS_ASSIGNED`. `ENUM_TO_STRING` adds an alias function `NAME_to_string(int)` — a more readable name for logging and printing.

```c
ENUMS_AUTOMATIC(DIRECTION);
ENUM_TO_STRING(DIRECTION);

// Both work identically:
DIRECTION_get_label(NORTH);   // "NORTH"
DIRECTION_to_string(NORTH);   // "NORTH"
```

Both functions return `NULL` for values that are not in the enum. Always check before printing:

```c
const char* name = DIRECTION_to_string(user_input);
if (name)
    printf("Direction: %s\n", name);
else
    printf("Unknown direction: %d\n", user_input);
```

---

## Part 4: Parallel Data Arrays — `ENUMS_ARRAY`

`ENUMS_ARRAY` generates a typed getter function that maps an enum value to one column of associated data. The data list is a separate macro — you can have one list per enum or reuse it for multiple extractions.

### Signature

```c
ENUMS_ARRAY(enum_name, data_list, return_type, suffix [, index])
```

Generates: `static inline return_type enum_name_get_suffix(int value)`

- `index` selects which column to extract from the data tuple (0-based, defaults to 0)
- Returns `(return_type)0` — i.e. `0`, `NULL`, `0.0f` — for out-of-bounds values

### Example: single attribute

```c
#define PLANET_ENUM(X, G)                 \
    X(G, MERCURY, 0.38f)                  \
    X(G, VENUS,   0.90f)                  \
    X(G, EARTH,   1.00f)                  \
    X(G, MARS,    0.38f)

ENUMS_AUTOMATIC(PLANET);
ENUMS_ARRAY(PLANET, PLANET_ENUM, float, gravity);  // index defaults to 0

printf("%.2f\n", PLANET_get_gravity(EARTH));  // 1.00
printf("%.2f\n", PLANET_get_gravity(MARS));   // 0.38
printf("%.2f\n", PLANET_get_gravity(99));     // 0.00  (out of bounds)
```

> `ENUMS_AUTOMATIC(PLANET)` looks for a macro named exactly `PLANET_ENUM`. The extra data columns are silently ignored when generating the enum values — only the member name is used. This means you can put all your data in `PLANET_ENUM` and use it for both enum generation and array extraction.

### Example: multiple attributes from one list

The same list macro is passed to `ENUMS_ARRAY` multiple times with different index values to extract each column independently:

```c
//                  name    gravity  diameter_km  moons
#define PLANET_ENUM(X, G)                                  \
    X(G, MERCURY, 0.38f, 4879,   0)                        \
    X(G, VENUS,   0.90f, 12104,  0)                        \
    X(G, EARTH,   1.00f, 12742,  1)                        \
    X(G, MARS,    0.38f, 6779,   2)

ENUMS_AUTOMATIC(PLANET);
ENUMS_ARRAY(PLANET, PLANET_ENUM, float, gravity);        // index 0 → gravity
ENUMS_ARRAY(PLANET, PLANET_ENUM, int,   diameter, 1);    // index 1 → diameter
ENUMS_ARRAY(PLANET, PLANET_ENUM, int,   moons,    2);    // index 2 → moons
ENUM_TO_STRING(PLANET);

for (int i = ENUM_BEGIN(PLANET); i < ENUM_END(PLANET); i++) {
    printf("%-8s  gravity=%.2fg  diameter=%dkm  moons=%d\n",
        PLANET_to_string(i),
        PLANET_get_gravity(i),
        PLANET_get_diameter(i),
        PLANET_get_moons(i));
}
```

Output:
```
MERCURY   gravity=0.38g  diameter=4879km   moons=0
VENUS     gravity=0.90g  diameter=12104km  moons=0
EARTH     gravity=1.00g  diameter=12742km  moons=1
MARS      gravity=0.38g  diameter=6779km   moons=2
```

### Index reference

For a data list entry like `X(G, MEMBER, val0, val1, val2, val3, ...)`:

| Index | Extracts |
|-------|----------|
| 0 (default) | `val0` |
| 1 | `val1` |
| 2 | `val2` |
| ... | ... |
| 10 | `val10` |

The member name (`MEMBER`) is never part of the index — it is always position zero of the macro call but is excluded from the data extraction count.

---

## Part 5: Combined Enum + Array — `ENUMS_MAP`

`ENUMS_MAP` is a shorthand for generating an enum and one parallel array in a single call. It is equivalent to calling `ENUMS_AUTOMATIC` (or `ENUMS_ASSIGNED`) followed by `ENUMS_ARRAY`.

```c
ENUMS_MAP(enum_name, data_list, generator, return_type, suffix)
```

- `generator` controls how enum values are assigned: use `ENUM` for automatic (0, 1, 2...) or `ENUM_VALUE_ASSIGN` to bind the first data column as the enum value.

### Example: automatic values with a mapped code

```c
#define KEY_ENUM(X, G)        \
    X(G, ENTER,  13)          \
    X(G, ESCAPE, 27)          \
    X(G, SPACE,  32)

// ENUM generator → ENTER=0, ESCAPE=1, SPACE=2 (sequential)
// ENUMS_ARRAY at index 0 → get_keycode returns 13, 27, 32
ENUMS_MAP(KEY, KEY_ENUM, ENUM, int, keycode);
ENUM_TO_STRING(KEY);

printf("%s → keycode %d\n", KEY_to_string(ENTER), KEY_get_keycode(ENTER));
// ENTER → keycode 13
```

### Example: assigned values with parallel data

When using `ENUM_VALUE_ASSIGN`, index 0 in the data tuple is the integer assigned to the enum member — so `ENUMS_MAP` at index 0 would just return that same integer back. For assigned enums with additional string or data columns, use `ENUMS_ASSIGNED` followed by `ENUMS_ARRAY` targeting the right index:

```c
#define SIGNAL_ENUM(X, G)                     \
    X(G, SIGTERM, 15, "terminate", "normal")  \
    X(G, SIGKILL, 9,  "kill",      "force")   \
    X(G, SIGHUP,  1,  "hangup",    "normal")

// ENUM_VALUE_ASSIGN → SIGTERM=15, SIGKILL=9, SIGHUP=1
ENUMS_ASSIGNED(SIGNAL);
ENUMS_ARRAY(SIGNAL, SIGNAL_ENUM, const char*, name,     1);  // index 1 → "terminate"
ENUMS_ARRAY(SIGNAL, SIGNAL_ENUM, const char*, category, 2);  // index 2 → "normal"

printf("Signal %d: %s (%s)\n", SIGKILL,
    SIGNAL_get_name(SIGKILL),      // kill
    SIGNAL_get_category(SIGKILL)); // force
```

---

## Part 6: Safety and Validation

### `ENUM_IS_VALID`

Returns 1 if the integer is a known enum value, 0 otherwise. Works correctly with sparse enums — it does not just check a range, it checks whether the value maps to a label.

```c
ENUM_IS_VALID(HTTP, 200)  // 1
ENUM_IS_VALID(HTTP, 201)  // 0  (gap in assigned enum)
ENUM_IS_VALID(HTTP, -1)   // 0
ENUM_IS_VALID(HTTP, 501)  // 0  (HTTP_total, the sentinel, is not a valid member)
```

Use it before casting untrusted integers to your enum type:

```c
int code = read_from_network();
if (!ENUM_IS_VALID(HTTP, code)) {
    fprintf(stderr, "Unknown HTTP code: %d\n", code);
    return -1;
}
HTTP status = (HTTP)code;
```

### `ENUM_SAFE_ARRAY_ACCESS`

Bounds-checks an index against a named enum before indexing a pointer array. Returns `NULL` if the index is invalid.

```c
const char* descriptions[] = { "success", "not found", "server error" };

// WRONG: no bounds checking
const char* msg = descriptions[user_value];

// CORRECT: returns NULL for any invalid user_value
const char* msg = ENUM_SAFE_ARRAY_ACCESS(descriptions, HTTP, user_value);
if (msg) printf("%s\n", msg);
```

> **Note:** `ENUM_SAFE_ARRAY_ACCESS` is only valid for pointer-type arrays. It returns `NULL` on failure; it cannot return a sentinel value for `int[]` or `float[]` arrays. For non-pointer arrays, use `ENUM_IS_VALID` to guard the access explicitly.

### `ENUM_COUNT` and `ENUM_TOTAL`

```c
ENUM_COUNT(HTTP)  // 3  — always the number of members defined
ENUM_TOTAL(HTTP)  // 501 — highest assigned value + 1; the _total sentinel member
```

`_count` is computed at compile time using a `sizeof` trick on a compound literal — it is always exact regardless of gaps or ordering. `_total` is the synthetic sentinel appended to the enum definition; for assigned enums it is `highest_value + 1`, not the member count.

---

## Part 7: Range Iteration — `ENUM_BEGIN` / `ENUM_END`

These macros provide a standard loop boundary for iterating over an enum's value range.

```c
ENUM_BEGIN(NAME)  // always expands to 0
ENUM_END(NAME)    // expands to NAME_total (highest value + 1)
```

For automatic enums (contiguous values from 0) this is equivalent to `for (int i = 0; i < COUNT; i++)`. For sparse assigned enums, combine with `ENUM_IS_VALID` to skip gaps:

```c
// Automatic enum — no gaps, no guard needed
for (int i = ENUM_BEGIN(DIRECTION); i < ENUM_END(DIRECTION); i++)
    printf("%s\n", DIRECTION_to_string(i));

// Sparse enum — guard required
for (int i = ENUM_BEGIN(HTTP); i < ENUM_END(HTTP); i++) {
    if (!ENUM_IS_VALID(HTTP, i)) continue;
    printf("%d → %s\n", i, HTTP_to_string(i));
}
```

---

## Part 8: Designated Initializer Arrays — `ENUM_VALUE_MAP`

`ENUM_VALUE_MAP` is a lower-level generator that produces C99 designated initializer syntax. Use it when you need a static array rather than a function, and you want the enum values as array indices.

```c
// The list macro must be named COLOR_ENUM for ENUMS_AUTOMATIC(COLOR) to find it.
// Extra data columns are ignored when generating the enum type.
#define COLOR_ENUM(X, G)       \
    X(G, RED,   "#FF0000")     \
    X(G, GREEN, "#00FF00")     \
    X(G, BLUE,  "#0000FF")

ENUMS_AUTOMATIC(COLOR);  // generates RED=0, GREEN=1, BLUE=2

// Produces: { [RED]="#FF0000", [GREEN]="#00FF00", [BLUE]="#0000FF" }
static const char* hex[COLOR_total] = {
    COLOR_ENUM(X_COMMA_, ENUM_VALUE_MAP)
};

printf("%s\n", hex[GREEN]);  // #00FF00
```

This is useful when you need a plain array (e.g. to pass to a function expecting `const char**`) rather than a getter function. Unlike `ENUMS_ARRAY`, it uses designated initializers so sparse enum values are handled correctly — unoccupied indices are zero-initialized by C99 rules.

> **Note:** `X_COMMA_` is an internal library macro used here directly. This is the one case where an internal name appears in user code. A future release may provide a cleaner public wrapper.

---

## Part 9: Generated API Reference

For an enum named `NAME` generated by any of the core macros:

| Symbol | Type | Description |
|--------|------|-------------|
| `NAME` | `typedef enum` | The enum type itself |
| `NAME_total` | `NAME` (enum member) | Sentinel; equals highest value + 1 |
| `NAME_count` | `static const int` | Exact number of defined members |
| `NAME_get_label(int)` | `const char*` | Label string, or `NULL` if invalid |
| `NAME_to_string(int)` | `const char*` | Alias for `_get_label`; requires `ENUM_TO_STRING(NAME)` |
| `NAME_get_SUFFIX(int)` | varies | Parallel data getter; requires `ENUMS_ARRAY` or `ENUMS_MAP` |

Utility macros (work on any generated enum):

| Macro | Returns | Description |
|-------|---------|-------------|
| `ENUM_IS_VALID(NAME, val)` | `int` (0 or 1) | 1 if `val` is a defined enum value |
| `ENUM_COUNT(NAME)` | `int` | Number of members |
| `ENUM_TOTAL(NAME)` | `int` | Highest value + 1 |
| `ENUM_BEGIN(NAME)` | `int` | Always 0; for loop start |
| `ENUM_END(NAME)` | `int` | `NAME_total`; for loop end |
| `ENUM_SAFE_ARRAY_ACCESS(arr, NAME, idx)` | pointer or `NULL` | Bounds-checked pointer array access |

---

## Full Example: Network Protocol Codes

This example ties all features together: assigned values, multiple parallel arrays, string conversion, and safe iteration.

```c
#include <stdio.h>
#include "enumfactorymacros.h"

/*
 * PROTO_ENUM columns:
 *   X(G, MEMBER, code, name_string, timeout_ms, retryable)
 */
#define PROTO_ENUM(X, G)                                          \
    X(G, PROTO_CONNECT,   1, "CONNECT",   5000, 1)               \
    X(G, PROTO_AUTH,      2, "AUTH",      3000, 1)               \
    X(G, PROTO_SEND,      3, "SEND",      1000, 1)               \
    X(G, PROTO_RECV,      4, "RECV",      2000, 1)               \
    X(G, PROTO_CLOSE,     5, "CLOSE",     500,  0)               \
    X(G, PROTO_HEARTBEAT, 9, "HEARTBEAT", 100,  0)

/*
 * Generated Enum: PROTO
 * -------------------------
 * Type: Assigned
 * Member Count: 6
 * Range: 0 to 10 (exclusive)
 *
 * Members: PROTO_CONNECT=1, PROTO_AUTH=2, PROTO_SEND=3,
 *          PROTO_RECV=4, PROTO_CLOSE=5, PROTO_HEARTBEAT=9
 */
ENUMS_ASSIGNED(PROTO);
ENUM_TO_STRING(PROTO);

ENUMS_ARRAY(PROTO, PROTO_ENUM, const char*, name,    1);  // index 1 → name string
ENUMS_ARRAY(PROTO, PROTO_ENUM, int,         timeout, 2);  // index 2 → timeout_ms
ENUMS_ARRAY(PROTO, PROTO_ENUM, int,         retry,   3);  // index 3 → retryable

void dispatch(int raw_opcode) {
    if (!ENUM_IS_VALID(PROTO, raw_opcode)) {
        fprintf(stderr, "Unknown opcode: %d\n", raw_opcode);
        return;
    }

    printf("%-12s  timeout=%dms  %s\n",
        PROTO_get_name(raw_opcode),
        PROTO_get_timeout(raw_opcode),
        PROTO_get_retry(raw_opcode) ? "retryable" : "no-retry");
}

int main(void) {
    printf("Member count: %d\n\n", ENUM_COUNT(PROTO));

    // Iterate all defined members, skipping gaps (6 is not defined)
    for (int i = ENUM_BEGIN(PROTO); i < ENUM_END(PROTO); i++) {
        if (ENUM_IS_VALID(PROTO, i))
            dispatch(i);
    }

    printf("\n--- Simulating received opcodes ---\n");
    int incoming[] = { 3, 9, 7, 1 };   // 7 is not a valid opcode
    for (int i = 0; i < 4; i++)
        dispatch(incoming[i]);

    return 0;
}
```

Output:

```
Member count: 6

CONNECT      timeout=5000ms  retryable
AUTH         timeout=3000ms  retryable
SEND         timeout=1000ms  retryable
RECV         timeout=2000ms  retryable
CLOSE        timeout=500ms   no-retry
HEARTBEAT    timeout=100ms   no-retry

--- Simulating received opcodes ---
SEND         timeout=1000ms  retryable
HEARTBEAT    timeout=100ms   no-retry
Unknown opcode: 7
CONNECT      timeout=5000ms  retryable
```

---

## Defining the Enum List: Rules and Limits

- No trailing commas or semicolons inside the `#define` body
- No limit on the number of members
- Up to 10 data attributes per member (indices 0–10)
- Member names occupy the global C namespace — prefix them to avoid collisions (`HTTP_OK`, not just `OK`)
- Assigned values must fit in `int`; `_total` is the highest value plus one

---

## Building and Testing

```bash
make          # builds libenumfactory.a and the test binary
make test     # compiles and runs tests/enumfactory_test.c
make clean    # removes build artifacts
```

Compiler: GCC with `-Wall -Wextra -O2`. The library has no dependencies beyond the C standard library.

---

## License

MIT. See `LICENSE` for details.
