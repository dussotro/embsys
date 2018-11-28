#include <stdio.h>
#include <stdlib.h>

// Question 8 : write here the buggy function without errors
int knot_to_kmh_str(float not, size_t size, char * format, char * kmh_str)
{
    float kmh = KNOT_TO_KMH * not;
    snprintf(kmh_str, size, format, kmh);

#ifndef GPS_OK
    iteration++;
    if (iteration == 2)
    {
    	nullPtr = NULL;
        puts("%s\n",nullPtr);
    }
#endif

    return kmh;
}
// Question 12 : write printf with a signal handler
