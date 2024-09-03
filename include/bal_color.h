#ifndef BAL_COLOR_H
#define BAL_COLOR_H
#include "bal_types.h"
#define ColorCreate(red, green, blue) ((Color){.r=(UByte)(red),.g=(UByte)(green),.b=(UByte)(blue)})

typedef struct Color_t
{
    UByte r;
    UByte g;
    UByte b;
} Color, *PColor;

#endif