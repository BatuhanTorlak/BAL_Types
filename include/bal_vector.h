#ifndef BAL_VECTOR_H
#define BAL_VECTOR_H
#include "bal_types.h"

// ---------------------------------------------------------
// Vector 2
// ---------------------------------------------------------

#define Vector2Create(X, Y) (Vector2){.x=(Float)(X), .y=(Float)(Y)}
#define Vector2Add(left, right) (Vector2){.x=(Float)(left.x + right.x), .y=(Float)(left.y + right.y)}
#define Vector2Sub(left, right) (Vector2){.x=(Float)(left.x - right.x), .y=(Float)(left.y - right.y)}
#define Vector2Mul(left, right) (Vector2){.x=(Float)(left.x * right.x), .y=(Float)(left.y * right.y)}
#define Vector2Div(left, right) (Vector2){.x=(Float)(left.x / right.x), .y=(Float)(left.y / right.y)}

typedef struct Vector2_t
{
    Float x;
    Float y;
}Vector2;

Float Vector2Distance(Vector2 start, Vector2 end);
Float Vector2Dot(Vector2 first, Vector2 second);
Float Vector2Cross(Vector2 first, Vector2 second);

// ---------------------------------------------------------
// Vector 3
// ---------------------------------------------------------

#define Vector3Create(X, Y, Z) (Vector3){.x=(Float)(X), .y=(Float)(Y), .z=(Float)(Z)}
#define Vector3Add(left, right) (Vector3){.x=(Float)(left.x + right.x), .y=(Float)(left.y + right.y), .z=(Float)(left.z + right.z)}
#define Vector3Sub(left, right) (Vector3){.x=(Float)(left.x - right.x), .y=(Float)(left.y - right.y), .z=(Float)(left.z - right.z)}
#define Vector3Mul(left, right) (Vector3){.x=(Float)(left.x * right.x), .y=(Float)(left.y * right.y), .z=(Float)(left.z * right.z)}
#define Vector3Div(left, right) (Vector3){.x=(Float)(left.x / right.x), .y=(Float)(left.y / right.y), .z=(Float)(left.z / right.z)}

typedef struct Vector3_t
{
    Float x;
    Float y;
    Float z;
}Vector3;

Float Vector3Distance(Vector3 start, Vector3 end);

#endif // VECTOR_H