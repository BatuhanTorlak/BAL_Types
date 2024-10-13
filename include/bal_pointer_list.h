#ifndef BAL_POINTER_LIST
#define BAL_POINTER_LIST

#include "bal_types.h"

typedef struct PointerList_t
{
    pvoid* ptr;
    ULong capacity;
    ULong count;
} PointerList, *PPointerList;

typedef Int(*PointerListForEach)(pvoid);

PPointerList PListCreate(ULong capacity);
void PListAdd(PPointerList list, const pvoid ptr);
Long PListFirst(const PPointerList list, const pvoid cmpr);
void PListForEach(const PPointerList list, const PointerListForEach forEach);
void PListRemoveAt(PPointerList list, const ULong index);
void PListRemoveAtA(PPointerList list, const ULong index);
ULong PListSize(const PPointerList list);
ULong PListCapacity(const PPointerList list);
pvoid PListGetIndex(const PPointerList list, const ULong index);
void PListSetIndex(PPointerList list, const ULong index, const pvoid ptr);
PPointerList PListClone(const PPointerList list);
void PListDestroy(const PPointerList ptr);
void PListDestroyAndClear(const PPointerList ptr);

#endif