#ifndef BSTRING_H
#define BSTRING_H
#include "bal_types.h"
#include "bal_pointer_list.h"

typedef struct BString_t
{
    PChar16 ptr;
    ULong len;
    ULong capacity;
}BString16, *PBString16;

void BalMemMove(PVoid source, PVoid dest, ULong size);

PBString16 BString16Create();
PBString16 BString16CreateA(const PChar16 ptr);
PBString16 BString16CreateB(const PChar16 ptr, const ULong length);
PBString16 BString16CreateC(const PChar16 ptr, const ULong length, const ULong capacity);
PBString16 BString16CreateD(const ULong capacity);
BString16 BString16CreateLocal(const PChar16 ptr);
BString16 BString16CreateLocalA(const PChar16 ptr, const ULong size);
void BString16ResetCapacity(PBString16 str, const ULong newCapacity);
ULong BString16Length(const PBString16 str);
Char16 BString16Get(const PBString16 str, const ULong index);
void BString16Set(PBString16 str, const ULong index, Char16 chr);
Bool BString16StartsWith(const PBString16 str, const PBString16 cmpr);
Bool BString16StartsWithA(const PBString16 str, const Char16 word);
Bool BString16EndsWith(const PBString16 str, const PBString16 cmpr);
Bool BString16EndsWithA(const PBString16 str, const Char16 word);
Bool BString16Compare(const PBString16 str, const PBString16 cmpr);
Bool BString16Contains(const PBString16 str, const PBString16 cmpr);
Bool BString16ContainsA(const PBString16 str, const Char16 word);
Bool BString16IsEmpty(const PBString16 str);
PBString16 BString16Concat(const PBString16 first, const PBString16 second);
PBString16 BString16ConcatA(const PBString16 list, const ULong count);
PBString16 BString16Substring(const PBString16 str, const ULong start, const ULong length);
PBString16 BString16SubstringA(const PBString16 str, const ULong start);
PBString16 BString16SubstringB(const PBString16 str, const ULong end);
PBString16 BString16SubstringC(const PBString16 str, const ULong start, const ULong end);
Long BString16First(const PBString16 str, const PBString16 frt);
Long BString16FirstA(const PBString16 str, const Char16 frt);
void BString16Trim(const PBString16 str);
void BString16TrimStart(const PBString16 str);
void BString16TrimEnd(const PBString16 str);
void BString16Replace(const PBString16 str, const Char16 replaced, const Char16 replacing);
void BString16Append(PBString16 str, const PBString16 apnd);
void BString16AppendA(PBString16 str, Char16 apnd);
void BString16AppendB(PBString16 str, PChar16 apnd);
void BString16AppendC(PBString16 str, PChar16 apnd, const ULong length);
void BString16Copy(const PBString16 from, const PBString16 to);
void BString16CopyA(const PBString16 from, const PBString16 to, const ULong length);
void BString16CopyB(const PBString16 from, const PBString16 to, const ULong startFrom, const ULong startTo, const ULong length);
void BString16CopyC(const PShort from, PShort to, const ULong length);
void BString16CopyD(const PShort from, PShort to, const ULong length);
PPointerList BString16Split(const PBString16 str, const Char16 wrd);
//PPointerList BStringSplitA(const PBString16 str, const PChar16 wrdList, const ULong length);
//PPointerList BStringSplitB(const PBString16 str, const PPointerList wrdList);
void BString16Clear(PBString16 str);
PBString16 BString16Clone(const PBString16 str);
void BString16Destroy(const PBString16 str);

#endif
