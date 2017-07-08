/*
 * =====================================================================================
 *
 *       Filename:  enumfactory.h
 *
 *    Description:  Enum macros 
 *
 *        Version:  1.0
 *        Created:  08/07/2017 03:00:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef  __ENUMFACTORY_H__
#define  __ENUMFACTORY_H__


/*-----------------------------------------------------------------------------
 *  
 * Base enum  facory definitions  
 *
 *-----------------------------------------------------------------------------*/

/* Single enumerator's member */
#define ENUM(_,...) _

/* Single enumerator as array index   */
#define ENUM_INDEX(_,...) [_] 

/* Single enumerator's member  as string */
#define ENUM_STRING(_,...) #_ 

/* Single enumerator's member with custom value */
#define ENUM_VALUE_ASSIGN(_, _v,...)  _= _v

#define ENUM_VALUE_MAP(_,_v,...)  ENUM_VALUE_ASSIGN(ENUM_INDEX(_), _v)
/* Single enumerator's memeber to value string  hash realation  */

#define ENUM_STRING_VALUE_MAP(_, _v,...)  ENUM_VALUE_ASSIGN(ENUM_INDEX(_),ENUM_STRING(_v))  /* [ENUM]="VALUE" */

#define ENUM_STRING_SELF_MAP(_,...)       ENUM_STRING_VALUE_MAP(_,_)

/*  Definition for total number of enum members  */
#define ENUM_TOTAL(_) _ ## _total

/*  Generate enumaration table with addional value ( XXXXX_total ) as total of enumarator's members */
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
};

/*-----------------------------------------------------------------------------
 *  
 *  Genrator implementaion
 *
 *-----------------------------------------------------------------------------*/
/* Base generators  */
#define ENUMS(_enum, _generator)                                          _generate_enums(_enum, _generator)
#define ENUMS_ARRAY(_enum, _array_type, _array_suffix, _array_generator)  _generate_enums_array(_enum, _array_type, _array_suffix, _array_generator)

/* Generetor for enums and enum's map   */
#define ENUMS_MAP(_enum, _enum_generator, _array_type, _array_suffix, _array_generator) \
        ENUMS(_enum, _enum_generator ) \
        ENUMS_ARRAY(_enum, _array_type, _array_suffix, _array_generator)


#define ENUMS_BASE(_, _enum_generator) ENUMS_MAP(_, _enum_generator,  char*, label, ENUM_STRING_SELF_MAP)
/*  
 *
 *  Generaye enums and array like XXXX_label[enum] = "enum"
 *
 */

#define ENUMS_REGULAR(_) ENUMS_BASE(_, ENUM)
#define ENUMS_ASIGNED(_) ENUMS_BASE(_, ENUM_VALUE_ASSIGN)



#endif   
