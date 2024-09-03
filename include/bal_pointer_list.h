#ifndef BAL_POINTER_LIST
#define BAL_POINTER_LIST

#include "bal_types.h"

typedef struct PointerList_t
{
    pvoid* ptr;
    Int capacity;
    Int count;
} PointerList, *PPointerList;

PPointerList PListCreate(Int capacity);
void PListAdd(PPointerList list, pvoid ptr);
void PListRemoveAt(PPointerList list, Int index);
void PListRemoveAtA(PPointerList list, Int index);
pvoid PListSetIndex(PPointerList list, Int index, pvoid ptr);
void PListDestroy(PPointerList ptr);
void PListDestroyAndClear(PPointerList ptr);

#endif