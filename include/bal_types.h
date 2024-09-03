#ifndef BAL_TYPES_H
#define BAL_TYPES_H

#define POINTER_SIZE sizeof(void*)
#define CHAR_SIZE sizeof(Char)

typedef unsigned int long long  ULong, uint64, 
                                *PULong, *puint64;


typedef int long long           Long, int64, bool64, Long,
                                *PLong, *pint64, *pbool64;


typedef int                     Int, int32, bool32, Bool, 
                                *PInt, *pint32, *pbool32, *PBool;


typedef unsigned int            UInt, uint32, 
                                *PUInt, *puint32;


typedef unsigned short          uint16, UShort, 
                                *puint16, *PUShort;


typedef short                   int16, Short, Char16, bool16,
                                *pint16, *PShort, *PChar16, *pbool16;


typedef char                    Byte, Char8, Char, bool8, 
                                *PByte, *PChar8, *PChar, *pbool, *pchar;


typedef unsigned char           UByte, UChar8, UChar, 
                                *PUByte, *PUChar8, *PUChar, *puchar;


typedef float                   float32, Float32, Float,
                                *pfloat32, *PFloat32;


typedef double                  float64, Float64,
                                *pfloat64, *PFloat64;


typedef void*                   pvoid;

#endif
