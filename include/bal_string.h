#ifndef BSTRING_H
#define BSTRING_H
#include "bal_types.h"
#include "bal_pointer_list.h"

typedef struct BString_t
{
    PChar ptr;
    ULong len;
    ULong capacity;
}BString, *PBString;

void BalMemMove(PVoid source, PVoid dest, ULong size);

PBString BStringCreate();
PBString BStringCreateA(const PChar ptr);
PBString BStringCreateB(const PChar ptr, const ULong length);
PBString BStringCreateC(const PChar ptr, const ULong length, const ULong capacity);
PBString BStringCreateD(const ULong capacity);
BString BStringCreateLocal(const PChar ptr);
BString BStringCreateLocalA(const PChar ptr, const ULong size);
void BStringResetCapacity(PBString str, const ULong newCapacity);
ULong BStringLength(const PBString str);
Char BStringGet(const PBString str, const ULong index);
void BStringSet(PBString str, const ULong index, Char chr);
Bool BStringStartsWith(const PBString str, const PBString cmpr);
Bool BStringStartsWithA(const PBString str, const Char word);
Bool BStringEndsWith(const PBString str, const PBString cmpr);
Bool BStringEndsWithA(const PBString str, const Char word);
Bool BStringCompare(const PBString str, const PBString cmpr);
Bool BStringContains(const PBString str, const PBString cmpr);
Bool BStringContainsA(const PBString str, const Char word);
Bool BStringIsEmpty(const PBString str);
PBString BStringConcat(const PBString first, const PBString second);
PBString BStringConcatA(const PBString list, const ULong count);
PBString BStringSubstring(const PBString str, const ULong start, const ULong length);
PBString BStringSubstringA(const PBString str, const ULong start);
PBString BStringSubstringB(const PBString str, const ULong end);
PBString BStringSubstringC(const PBString str, const ULong start, const ULong end);
Long BStringFirst(const PBString str, const PBString frt);
Long BStringFirstA(const PBString str, const Char frt);
void BStringTrim(const PBString str);
void BStringTrimStart(const PBString str);
void BStringTrimEnd(const PBString str);
void BStringReplace(const PBString str, const Char replaced, const Char replacing);
void BStringAppend(PBString str, const PBString apnd);
void BStringAppendA(PBString str, Char apnd);
void BStringAppendB(PBString str, PChar apnd);
void BStringAppendC(PBString str, PChar apnd, const ULong length);
void BStringCopy(const PBString from, const PBString to);
void BStringCopyA(const PBString from, const PBString to, const ULong length);
void BStringCopyB(const PBString from, const PBString to, const ULong startFrom, const ULong startTo, const ULong length);
void BStringCopyC(const PByte from, PByte to, const ULong length);
void BStringCopyD(const PByte from, PByte to, const ULong length);
PPointerList BStringSplit(const PBString str, const Char wrd);
//PPointerList BStringSplitA(const PBString str, const PChar wrdList, const ULong length);
//PPointerList BStringSplitB(const PBString str, const PPointerList wrdList);
void BStringClear(PBString str);
PBString BStringClone(const PBString str);
void BStringDestroy(const PBString str);

#endif
