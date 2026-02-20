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
#ifndef __ENUMFACTORYMACROS_H__
#define __ENUMFACTORYMACROS_H__

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
 * ENUM_STRING_SELF_MAP: Generates a string mapping of the identifier itself.
 *       Example expansion: [RED] = "RED"
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

/* Zero padding avoids compiler errors if generators don't have enough arguments */
#define ENUM_SWITCH_CASE_VAL_0(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_0(__VA_ARGS__, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_1(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_1(__VA_ARGS__, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_2(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_2(__VA_ARGS__, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_3(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_3(__VA_ARGS__, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_4(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_4(__VA_ARGS__, 0, 0, 0, 0, 0, 0);
#define ENUM_SWITCH_CASE_VAL_5(_1, ...) case _1: return ENUM_VALUE_MAP_VAL_5(__VA_ARGS__, 0, 0, 0, 0, 0, 0);

#define _ENUMS_CONCAT_IMPL(a, b) a ## b
#define _ENUMS_CONCAT(a, b) _ENUMS_CONCAT_IMPL(a, b)

/* Invokers
 * --------
 * These helper macros control how the generator list is expanded.
 *
 * _X_COMMA: Appends a comma after the generator call.
 *           Used inside enum definitions where members are comma-separated.
 *           Example: RED, GREEN, BLUE,
 *
 * _ENUM_VAL_COUNT: Returns 1 for each item, used for counting members.
 */
#define _X_CALL(_gen, ...) _gen(__VA_ARGS__)
#define _X_COMMA(_gen, ...) _gen(__VA_ARGS__),
#define _ENUM_VAL_COUNT(_1, ...) 1,

/* Unified Generation Macro
 * ------------------------
 * This is the heart of the library. It produces 4 artifacts for every enum:
 *
 * 1. The Enum Type definition (typedef enum { ... } Name;)
 *    - Uses _X_COMMA with the provided generator to list members.
 *    - Appends a _total member at the end to track range.
 *
 * 2. Members Count (_count)
 *    - A static const int that holds the actual number of defined items.
 *
 * 3. Label Array (_label)
 *    - An array of string literals mapping value -> identifier name.
 *    - Used for automatic to-string conversion.
 *
 * 4. Safe Label Accessor (_get_label)
 *    - A function to safely retrieve the label, returning NULL if out of bounds.
 */
#define GENERATE_ENUM_CORE(_enum_name, _enum_list, _generator) \
typedef enum { \
    _enum_list(_X_COMMA, _generator) \
    _enum_name ## _total \
} _enum_name; \
static const int _enum_name ## _count = (sizeof((int[]){ _enum_list(_ENUM_VAL_COUNT, 0) 0 }) / sizeof(int)) - 1; \
static inline const char* _enum_name ## _get_label(int value) { \
    switch(value) { \
        _enum_list(_X_CALL, ENUM_SWITCH_CASE_LABEL_) \
        default: return (const char*)0; \
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
 * ENUMS_ARRAY: Generates a lookup array for an existing map of values.
 *              Used when you want to map Enum -> Arbitrary Data.
 *              Accepts an optional 5th argument specifying the column/index
 *              within the generator macro tuple to extract (0-7, defaults to 0).
 *
 * ENUMS_MAP: A convenience wrapper that generates both the core enum
 *            AND a parallel data array in one shot.
 */
#define _ENUMS_ARRAY_IMPL(_enum_name, _enum_list, _type, _suffix, _index) \
static inline _type _enum_name ## _get_ ## _suffix(int value) { \
    switch(value) { \
        _enum_list(_X_CALL, _ENUMS_CONCAT(ENUM_SWITCH_CASE_VAL_, _index)) \
        default: return (_type)0; \
    } \
}

#define _ENUMS_ARRAY_4(enum_name, enum_list, type, suffix) \
    _ENUMS_ARRAY_IMPL(enum_name, enum_list, type, suffix, 0)

#define _ENUMS_ARRAY_5(enum_name, enum_list, type, suffix, index) \
    _ENUMS_ARRAY_IMPL(enum_name, enum_list, type, suffix, index)

#define _ENUMS_ARRAY_GET_MACRO(_1, _2, _3, _4, _5, NAME, ...) NAME

/* ENUMS_ARRAY accepts either 4 args (defaults to index 0) or 5 args (explicit index) */
#define ENUMS_ARRAY(...) \
    _ENUMS_ARRAY_GET_MACRO(__VA_ARGS__, _ENUMS_ARRAY_5, _ENUMS_ARRAY_4)(__VA_ARGS__)

#define ENUMS_MAP(_enum_name, _enum_list, _generator, _type, _suffix) \
GENERATE_ENUM_CORE(_enum_name, _enum_list, _generator) \
ENUMS_ARRAY(_enum_name, _enum_list, _type, _suffix)

/*-----------------------------------------------------------------------------
 * Safety and Validation Macros
 * Provides runtime safety checks and bounds validation
 *-----------------------------------------------------------------------------*/

#define ENUM_IS_VALID(_enum, _value) \
    (_enum ## _get_label(_value) != (const char*)0)

#define ENUM_TO_STRING(_enum) \
    static inline const char* _enum ## _get_label(int value) __attribute__((unused)); \
    static inline const char* _enum ## _to_string(int value) __attribute__((unused)); \
    static inline const char* _enum ## _to_string(int value) { \
        return _enum ## _get_label(value); \
    }

#define ENUM_BEGIN(_enum) ((int)0)
#define ENUM_END(_enum) (ENUM_TOTAL(_enum))

#define ENUM_SAFE_ARRAY_ACCESS(_array, _enum, _index) \
    (ENUM_IS_VALID(_enum, _index) ? _array[_index] : (void*)0)

#endif /* __ENUMFACTORYMACROS_H__ */
