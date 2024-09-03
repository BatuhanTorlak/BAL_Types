#ifndef BAL_POINT_H
#define BAL_POINT_H
#define Point2DCreate(X, Y) ((Point2D){.x=(Int)(X),.y=(Int)(Y)})
#define Point3DCreate(X, Y, Z) ((Point3D){.x=(Int)(X),.y=(Int)(Y),.z=(Int)(Z)})

#include "bal_types.h"

typedef struct Point2D_t
{
    Int x;
    Int y;
} Point2D, *PPoint2D;

typedef struct Point3D_t
{
    Int x;
    Int y;
    Int z;
} Point3D, *PPoint3D;

#endif