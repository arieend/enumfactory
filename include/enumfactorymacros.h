/*
 * =====================================================================================
 * EnumFactory - Advanced Enum Generation Macros
 * =====================================================================================
 *
 * A flexible enum generation system that provides:
 * - Automatic string conversion
 * - Value mapping
 * - Type-safe access
 * - Bounds checking
 * - Array generation
 *
 * Usage Example:
 * -------------
 * #define COLOR_ENUM(X, G) \
 *     X(G, RED), \
 *     X(G, GREEN), \
 *     X(G, BLUE)
 *
 * ENUMS_AUTOMATIC(COLOR);
 *
 * // Now you can use:
 * // COLOR_label[RED] -> "RED"
 * // ENUM_IS_VALID(COLOR, value) -> bounds checking
 * // COLOR_get_label(RED) -> safe access to label
 * =====================================================================================
 */

#pragma once

#include <stddef.h>

/*-----------------------------------------------------------------------------
 * Base Enum Generation Macros
 * These macros form the foundation of enum member generation
 *-----------------------------------------------------------------------------*/

/* Core Generators (No trailing commas)
 * -------------------------------------
 * These macros act as the "visitor" logic for each X-Macro expansion.
 *
 * ENUM: Generates just the identifier. Used for standard enum definition.
 *       Example expansion: RED
 *
 * ENUM_VALUE_ASSIGN: Generates identifier = value. Used for assigned enums.
 *       Example expansion: OK = 200
 *
 * ENUM_VALUE_MAP: Generates a designated initializer for array mapping.
 *       _1 is the enum key, VA_ARGS is the value.
 *       Example expansion: [RED] = "FF0000"
 *
 * ENUM_SWITCH_CASE_VAL_: Switch-case generator extracting the first variadic
 *       argument. Equivalent to ENUM_SWITCH_CASE_VAL_0.
 *       Example expansion: case RED: return "FF0000";
 */
#define ENUM(_1, ...) _1
#define ENUM_VALUE_ASSIGN(_1, _2, ...) _1 = _2
#define ENUM_VALUE_MAP(_1, ...) [(_1)] = ENUM_VALUE_MAP_VAL_(__VA_ARGS__, _1)
#define ENUM_VALUE_MAP_VAL_(_val, ...) _val
#define ENUM_SWITCH_CASE_LABEL_(_1, ...) case _1: return #_1;
#define ENUM_SWITCH_CASE_VAL_(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_(__VA_ARGS__, _1);

#define ENUM_VALUE_MAP_VAL_0(_0, ...) _0
#define ENUM_VALUE_MAP_VAL_1(_0, _1, ...) _1
#define ENUM_VALUE_MAP_VAL_2(_0, _1, _2, ...) _2
#define ENUM_VALUE_MAP_VAL_3(_0, _1, _2, _3, ...) _3
#define ENUM_VALUE_MAP_VAL_4(_0, _1, _2, _3, _4, ...) _4
#define ENUM_VALUE_MAP_VAL_5(_0, _1, _2, _3, _4, _5, ...) _5
#define ENUM_VALUE_MAP_VAL_6(_0, _1, _2, _3, _4, _5, _6, ...) _6
#define ENUM_VALUE_MAP_VAL_7(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define ENUM_VALUE_MAP_VAL_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define ENUM_VALUE_MAP_VAL_9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define ENUM_VALUE_MAP_VAL_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10

/* Zero padding avoids compiler errors if generators don't have enough arguments */
#define ENUM_SWITCH_CASE_VAL_0(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_0(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_1(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_1(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_2(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_2(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_3(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_3(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_4(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_4(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_5(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_5(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_6(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_6(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_7(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_7(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_8(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_8(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_9(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_9(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_10(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_10(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

#define ENUMS_CONCAT_IMPL_(a, b) a ## b
#define ENUMS_CONCAT_(a, b) ENUMS_CONCAT_IMPL_(a, b)

/* Invokers
 * --------
 * These helper macros control how the generator list is expanded.
 *
 * X_COMMA_: Appends a comma after the generator call.
 *           Used inside enum definitions where members are comma-separated.
 *           Example: RED, GREEN, BLUE,
 *
 * ENUM_VAL_COUNT_: Returns 1 for each item, used for counting members.
 */
#define X_CALL_(_gen, ...) _gen(__VA_ARGS__)
#define X_COMMA_(_gen, ...) _gen(__VA_ARGS__),
#define ENUM_VAL_COUNT_(_1, ...) 1,

/* Unified Generation Macro
 * ------------------------
 * This is the heart of the library. It produces 3 artifacts for every enum:
 *
 * 1. The Enum Type definition (typedef enum { ... } Name;)
 *    - Uses X_COMMA_ with the provided generator to list members.
 *    - Appends a _total member at the end to track range.
 *
 * 2. Members Count (_count)
 *    - A static const int that holds the actual number of defined items.
 *
 * 3. Safe Label Accessor (_get_label)
 *    - A function to safely retrieve the string label of a value,
 *      returning NULL if out of bounds.
 */
#define GENERATE_ENUM_CORE(_enum_name, _enum_list, _generator) \
typedef enum { \
    _enum_list(X_COMMA_, _generator) \
    _enum_name ## _total \
} _enum_name; \
static const int _enum_name ## _count = (sizeof((int[]){ _enum_list(ENUM_VAL_COUNT_, 0) 0 }) / sizeof(int)) - 1; \
static inline const char* _enum_name ## _get_label(int value) { \
    switch(value) { \
        _enum_list(X_CALL_, ENUM_SWITCH_CASE_LABEL_) \
        default: return NULL; \
    } \
}

#define ENUM_TOTAL(_enum) _enum ## _total
#define ENUM_COUNT(_enum) _enum ## _count

/* ENUMS_AUTOMATIC:
 * Generates an enum where values are automatically assigned (0, 1, 2...).
 * Uses the ENUM generator which just outputs the name.
 */
#define ENUMS_AUTOMATIC(_enum_name) GENERATE_ENUM_CORE(_enum_name, _enum_name ## _ENUM, ENUM)

/* ENUMS_ASSIGNED:
 * Generates an enum where values are manually assigned.
 * Uses the ENUM_VALUE_ASSIGN generator which outputs 'Name = Value'.
 */
#define ENUMS_ASSIGNED(_enum_name) GENERATE_ENUM_CORE(_enum_name, _enum_name ## _ENUM, ENUM_VALUE_ASSIGN)

/* Specialized Map/Array Generation
 * ----------------------------------
 * These macros generate parallel arrays based on the enum structure.
 *
 * ENUMS_ARRAY: Generates a lookup function for an existing map of values.
 *              Used when you want to map Enum -> Arbitrary Data.
 *              Accepts an optional 5th argument specifying the column/index
 *              within the generator macro tuple to extract (0-10, defaults to 0).
 *
 * ENUMS_MAP: A convenience wrapper that generates both the core enum
 *            AND a parallel data array in one shot.
 */
#define ENUMS_ARRAY_IMPL_(_enum_name, _enum_list, _type, _suffix, _index) \
static inline _type _enum_name ## _get_ ## _suffix(int value) { \
    switch(value) { \
        _enum_list(X_CALL_, ENUMS_CONCAT_(ENUM_SWITCH_CASE_VAL_, _index)) \
        default: return (_type)0; \
    } \
}

#define ENUMS_ARRAY_4_(enum_name, enum_list, type, suffix) \
    ENUMS_ARRAY_IMPL_(enum_name, enum_list, type, suffix, 0)

#define ENUMS_ARRAY_5_(enum_name, enum_list, type, suffix, index) \
    ENUMS_ARRAY_IMPL_(enum_name, enum_list, type, suffix, index)

#define ENUMS_ARRAY_GET_MACRO_(_1, _2, _3, _4, _5, NAME, ...) NAME

/* ENUMS_ARRAY accepts either 4 args (defaults to index 0) or 5 args (explicit index) */
#define ENUMS_ARRAY(...) \
    ENUMS_ARRAY_GET_MACRO_(__VA_ARGS__, ENUMS_ARRAY_5_, ENUMS_ARRAY_4_)(__VA_ARGS__)

#define ENUMS_MAP(_enum_name, _enum_list, _generator, _type, _suffix) \
GENERATE_ENUM_CORE(_enum_name, _enum_list, _generator) \
ENUMS_ARRAY(_enum_name, _enum_list, _type, _suffix)

/*-----------------------------------------------------------------------------
 * Safety and Validation Macros
 * Provides runtime safety checks and bounds validation
 *-----------------------------------------------------------------------------*/

#define ENUM_IS_VALID(_enum, _value) \
    (_enum ## _get_label(_value) != NULL)

#define ENUM_TO_STRING(_enum) \
    static inline const char* _enum ## _to_string(int value) { \
        return _enum ## _get_label(value); \
    }

#define ENUM_BEGIN(_enum) ((int)0)
#define ENUM_END(_enum) (ENUM_TOTAL(_enum))

/* Only valid for pointer-type arrays; returns NULL for out-of-bounds indices. */
#define ENUM_SAFE_ARRAY_ACCESS(_array, _enum, _index) \
    (ENUM_IS_VALID(_enum, _index) ? _array[_index] : NULL)
