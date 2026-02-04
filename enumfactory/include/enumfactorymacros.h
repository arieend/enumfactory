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

/* Core Generators (No trailing commas) */
#define ENUM(_1, ...) _1
#define ENUM_VALUE_ASSIGN(_1, _2, ...) _1 = _2
#define ENUM_VALUE_MAP(_1, ...) [(_1)] = ENUM_VALUE_MAP_VAL_(__VA_ARGS__, _1)
#define ENUM_VALUE_MAP_VAL_(_val, ...) _val
#define ENUM_STRING_SELF_MAP(_1, ...) [(_1)] = #_1

/* Invokers */
#define _X_CALL(_gen, ...) _gen(__VA_ARGS__)
#define _X_COMMA(_gen, ...) _gen(__VA_ARGS__),
#define _ENUM_VAL_COUNT(_1, ...) 1,

/* Unified Generation Macro */
#define GENERATE_ENUM_CORE(_enum_name, _enum_list, _generator) \
typedef enum { \
    _enum_list(_X_COMMA, _generator) \
    _enum_name ## _total \
} _enum_name; \
static const int _enum_name ## _count __attribute__((unused)) = (sizeof((int[]){ _enum_list(_ENUM_VAL_COUNT) 0 }) / sizeof(int)) - 1; \
static const char* _enum_name ## _label[ _enum_name ## _total ] __attribute__((unused)) = { \
    _enum_list(_X_COMMA, ENUM_STRING_SELF_MAP) \
}; \
static inline const char* _enum_name ## _get_label(int value) __attribute__((unused)) { \
    return (value >= 0 && value < _enum_name ## _total) ? _enum_name ## _label[value] : (const char*)0; \
}

#define ENUM_TOTAL(_enum) _enum ## _total
#define ENUM_COUNT(_enum) _enum ## _count

#define ENUMS_AUTOMATIC(_enum_name) GENERATE_ENUM_CORE(_enum_name, _enum_name ## _ENUM, ENUM)
#define ENUMS_ASSIGNED(_enum_name) GENERATE_ENUM_CORE(_enum_name, _enum_name ## _ENUM, ENUM_VALUE_ASSIGN)

/* Specialized Map/Array Generation */
#define ENUMS_ARRAY(_enum_name, _enum_list, _type, _suffix) \
static const _type _enum_name ## _ ## _suffix[ _enum_name ## _total ] __attribute__((unused)) = { \
    _enum_list(_X_COMMA, ENUM_VALUE_MAP) \
}; \
static inline _type _enum_name ## _get_ ## _suffix(int value) __attribute__((unused)) { \
    return (value >= 0 && value < _enum_name ## _total) ? _enum_name ## _ ## _suffix[value] : (_type)0; \
}

#define ENUMS_MAP(_enum_name, _enum_list, _generator, _type, _suffix) \
GENERATE_ENUM_CORE(_enum_name, _enum_list, _generator) \
ENUMS_ARRAY(_enum_name, _enum_list, _type, _suffix)

/*-----------------------------------------------------------------------------
 * Safety and Validation Macros
 * Provides runtime safety checks and bounds validation
 *-----------------------------------------------------------------------------*/

#define ENUM_IS_VALID(_enum, _value) \
    ((_value) >= 0 && (_value) < ENUM_TOTAL(_enum) && _enum ## _get_label(_value) != (const char*)0)

#define ENUM_TO_STRING(_enum) \
    static inline const char* _enum ## _to_string(int value) __attribute__((unused)); \
    static inline const char* _enum ## _to_string(int value) { \
        return _enum ## _get_label(value); \
    }

#define ENUM_BEGIN(_enum) ((int)0)
#define ENUM_END(_enum) (ENUM_TOTAL(_enum))

#define ENUM_SAFE_ARRAY_ACCESS(_array, _enum, _index) \
    ((_index >= 0 && _index < ENUM_TOTAL(_enum)) ? _array[_index] : (void*)0)

#endif /* __ENUMFACTORYMACROS_H__ */
