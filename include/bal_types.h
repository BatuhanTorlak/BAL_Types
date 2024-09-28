#ifndef BAL_TYPES_H
#define BAL_TYPES_H

#define POINTER_SIZE sizeof(void*)
#define CHAR_SIZE sizeof(Char)

typedef unsigned int long long  ULong, UInt64, uint64, 
                                *PULong, *PUInt64, *puint64;


typedef int long long           Long, Int64, int64, bool64, Long,
                                *PLong, *PInt64, *pint64, *pbool64;


typedef int                     Int, Int32, int32, bool32, Bool, 
                                *PInt, *PInt32, *pint32, *pbool32, *PBool;


typedef unsigned int            UInt, UInt32, uint32, 
                                *PUInt, *PUInt32, *puint32;


typedef unsigned short          uint16, UInt16, UShort, 
                                *puint16, *PUInt16, *PUShort;


typedef short                   int16, Int16, Short, Char16, bool16,
                                *pint16, *PInt16, *PShort, *PChar16, *pbool16;


typedef char                    int8, Int8, Byte, Char8, Char, bool8, 
                                *pint8, *PInt8, *PByte, *PChar8, *PChar, *pbool, *pchar;


typedef unsigned char           uint8, UInt8, UByte, UChar8, UChar, 
                                *puint8, *PUInt8, *PUByte, *PUChar8, *PUChar, *puchar;


typedef float                   float32, Float32, Float,
                                *pfloat32, *PFloat32, *PFloat;


typedef double                  float64, Float64, Double,
                                *pfloat64, *PFloat64, *PDouble;

typedef __float128              float128, Float128, Quad,
                                *pfloat128, *PFloat128, *PQuad;

typedef struct IntVector2_t
{
    Long x;
    Long y;
} IntVector2, *PIntVector2;


typedef void*                   pvoid, *PVoid;

#endif
