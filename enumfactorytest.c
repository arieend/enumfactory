/*
 * =====================================================================================
 *
 *       Filename:  enumfactorytest.c
 *
 *    Description:  Test Enum  fctory definiton
 *
 *        Version:  1.0
 *        Created:  08/07/2017 03:04:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "enumfactorytest.h"

int main(int argc, const char *argv[])
{
int i;

/*  printf("TEST: %d, %d, %d\n", TEST1, TEST2, TEST3); 
 */
 for(i =0; i <  ENUM_TEST_total; i++)
 {
    if ( ENUM_TEST_label[i] )
    {
       printf("TEST: %d =  %s\n", i, ENUM_TEST_label[i]);
       fflush(stdout);
    }


 }


 return 0;
}

