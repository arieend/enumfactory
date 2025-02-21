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
 * #define COLOR_ENUM(GENERATOR) \
 *     GENERATOR(RED), \
 *     GENERATOR(GREEN), \
 *     GENERATOR(BLUE)
 *
 * ENUMS_AUTOMATIC(COLOR)
 * 
 * // Now you can use:
 * // COLOR_label[RED] -> "RED"
 * // ENUM_IS_VALID(COLOR, value) -> bounds checking
 * // COLOR_get_label(RED) -> safe access to label
 * =====================================================================================
 */

#pragma once
#ifndef  __ENUMFACTORYMACROS_H__
#define  __ENUMFACTORYMACROS_H__

/*-----------------------------------------------------------------------------
 * Base Enum Generation Macros
 * These macros form the foundation of enum member generation
 *-----------------------------------------------------------------------------*/

/* Basic enum member generation without value assignment
 * Args:
 *   _ : The enum member name
 *   ...: Variable arguments (unused, for compatibility) */
#define ENUM(_,...) _

/* Generate array index accessor for enum member
 * Args:
 *   _ : The enum member name to use as array index
 *   ...: Variable arguments (unused, for compatibility) */
#define ENUM_INDEX(_,...) [_] 

/* Enumerator's member  as string */
#define ENUM_STRING(_,...) #_ 

/* Enumerator's member with custom value */
#define ENUM_VALUE_ASSIGN(_, _v,...)  _= _v

/*-----------------------------------------------------------------------------
 *  Parameter documentation
 *  _ : enum member name
 *  _v: enum member value
 *  ...: additional parameters (unused)
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *  
 *  Enum implementation
 *
 *-----------------------------------------------------------------------------*/
/* Enumerator's member to value hash realation  */
#define ENUM_VALUE_MAP(_,_v,...)  ENUM_VALUE_ASSIGN(ENUM_INDEX(_), _v) /* [ENUM] = VALUE */

/* Enumerator's member to value hash realation as a string */
#define ENUM_STRING_VALUE_MAP(_, _v,...)  ENUM_VALUE_ASSIGN(ENUM_INDEX(_),ENUM_STRING(_v))  /* [ENUM]="VALUE" */

/* Map enumerator's member to its self as desrtiption */ 
#define ENUM_STRING_SELF_MAP(_,...)  ENUM_STRING_VALUE_MAP(_,_) /* [ENUM]="ENUM" */

/* Definition for total number of enum members  */
#define ENUM_TOTAL(_) _ ## _total

/* Generate enumeration table with addional value ( XXXXX_total ) as total of enumarator's members */
#define _generate_enums(_enum, _generator) \
typedef enum _ ## _enum \
             { \
               _enum(_generator), \
               ENUM_TOTAL(_enum) \
             } _enum;                          

/*  Generate array of values there  every enumerator member is the index 
 *  T-  type of array
 *  _enum ## _ ##  _suffix  - the name of array
 *  ENUM_TOTAL(_enum) -  the total numbers of array's elements from GENERATE_ENUMS defenition
 */ 

#define _generate_enums_array(_enum, T, _suffix, _generator)  \
__attribute__((unused)) static const T _enum ## _ ## _suffix[ENUM_TOTAL(_enum)] = { \
                        _enum(_generator), \
}; \
__attribute__((unused)) static inline T _enum ## _get_ ## _suffix(_enum value) { \
    return ENUM_IS_VALID(_enum, value) ? _enum ## _ ## _suffix[value] : NULL; \
}

/*-----------------------------------------------------------------------------
 *  
 *  Arrays' generator
 *
 *-----------------------------------------------------------------------------*/
/* Base generators  */
#define ENUMS(_enum, _generator)                                          _generate_enums(_enum, _generator)
#define ENUMS_ARRAY(_enum, _array_type, _array_suffix, _array_generator)  _generate_enums_array(_enum, _array_type, _array_suffix, _array_generator)

/* Generator for enums and enum's map   */
#define ENUMS_MAP(_enum, _enum_generator, _array_type, _array_suffix, _array_generator) \
        ENUMS(_enum, _enum_generator ) \
        ENUMS_ARRAY(_enum, _array_type, _array_suffix, _array_generator)


/*  
 *
 *  Generaye enums and array like XXXX_label[enum] = "enum"
 *
 *  ENUM_AUTOMATIC - Automatic enumarators valus        ex.: enum { ENUM0, ENUM1, ENUM2,... } 
 *  ENUM_ASSIGNED  - Assign value to every enum member  ex.: enum { ENUM0=10, ENUM1=20, ENUM2=30, ....} 
 */
#define ENUMS_BASE(_, _enum_generator) ENUMS_MAP(_, _enum_generator,  char*, label, ENUM_STRING_SELF_MAP)

#define ENUMS_AUTOMATIC(_) ENUMS_BASE(_, ENUM)               
#define ENUMS_ASSIGNED(_)  ENUMS_BASE(_, ENUM_VALUE_ASSIGN)  

/*-----------------------------------------------------------------------------
 * Safety and Validation Macros
 * Provides runtime safety checks and bounds validation
 *-----------------------------------------------------------------------------*/

/* Safe array access with bounds checking
 * Args:
 *   _array: The array to access
 *   _enum: The enum type
 *   _index: The index to access
 * Returns: Array element or NULL if out of bounds */
#define ENUM_SAFE_ARRAY_ACCESS(_array, _enum, _index) \
    ((_index >= 0 && _index < ENUM_TOTAL(_enum)) ? _array[_index] : NULL)

/* Enum range and validity checking utilities
 * These macros help with enum value validation and iteration */
#define ENUM_BEGIN(_enum) ((_enum)0)  // First valid enum value
#define ENUM_END(_enum) (ENUM_TOTAL(_enum))  // One past last valid enum value
#define ENUM_IS_VALID(_enum, _value) ((_value) >= 0 && (_value) < ENUM_TOTAL(_enum))

/* Convenience string conversion macro
 * Generates a type-safe conversion function from enum to string
 * Args:
 *   _enum: The enum type to generate converter for */
#define ENUM_TO_STRING(_enum) \
    static inline const char* _enum ## _to_string(_enum value) { \
        return _enum ## _get_label(value); \
    }

#endif   /*  __ENUMFACTORYMACROS_H__ */
