/*
 * =====================================================================================
 *
 *       Filename:  enumfactorytest.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  08/07/2017 13:05:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef   __ENUMFACTORYTEST_H__
#define   __ENUMFACTORYTEST_H__

#include "enumfactory.h"

#define ENUM_TEST(_)  \
        _(TEST1, 20), \
        _(TEST2, 30), \
        _(TEST3, 40)



/* ENUMS_AUTOMATIC(ENUM_TEST)
 */

ENUMS_ASSIGNED(ENUM_TEST)

#endif
