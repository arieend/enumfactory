#include <stdio.h>
#include <stdlib.h>

#include "enumfactory.h"

int printEnumLabels( int total,const char **array ) {
   
   for (int i = 0; i < total; i++)
   {
     if( !array[i] )
        continue;
     printf("Label[%d]: %s\n", i, array[i]);
     fflush(stdout);
   }
}