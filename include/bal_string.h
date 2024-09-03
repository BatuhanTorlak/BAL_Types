#ifndef BSTRING_H
#define BSTRING_H
#include "bal_types.h"

typedef struct BString_t
{
    PChar ptr;
    ULong len;
    ULong capacity;
}BString, *PBString;

PBString BStringCreate();
PBString BStringCreateA(const PChar ptr);
PBString BStringCreateB(const PChar ptr, const ULong length);
PBString BStringCreateC(const PChar ptr, const ULong length, const ULong capacity);
PBString BStringCreateD(const ULong capacity);
BString BStringCreateLocal(const PChar ptr);
BString BStringCreateLocalA(const PChar ptr, const ULong size);
void BStringResetCapacity(PBString str, const ULong newCapacity);
ULong BStringLength(const PBString str);
Bool BStringStartsWith(const PBString str, const PBString cmpr);
Bool BStringStartsWithA(const PBString str, const Char word);
Bool BStringEndsWith(const PBString str, const PBString cmpr);
Bool BStringEndsWithA(const PBString str, const Char word);
Bool BStringCompare(const PBString str, const PBString cmpr);
Bool BStringContains(const PBString str, const PBString cmpr);
Bool BStringContainsA(const PBString str, const Char word);
PBString BStringConcat(const PBString first, const PBString second);
PBString BStringConcatA(const PBString list, const ULong count);
void BStringAppend(PBString str, const PBString apnd);
void BStringAppendA(PBString str, Char apnd);
void BStringAppendB(PBString str, PChar apnd);
void BStringClear(PBString str);
PBString BStringClone(const PBString str);
void BStringDestroy(const PBString str);

#endif
