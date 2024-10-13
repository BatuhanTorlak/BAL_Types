#include "bal_string.h"
#include "bal_memory.h"
#include "bal_pointer_list.h"
#define PTR_LIST_DEFAULT_CAPACITY 6
#define SIZING(x) (sizeof(pvoid) * (x))

PPointerList PListCreate(ULong capacity)
{
    if (capacity == 0)
        capacity = PTR_LIST_DEFAULT_CAPACITY;
    PPointerList _p = BalAlloc(sizeof(PointerList));
    _p->capacity = capacity;
    _p->count = 0;
    _p->ptr = BalAlloc(SIZING(capacity));
    return _p;
}

void PListAdd(restrict PPointerList list, const restrict pvoid ptr)
{
    if (list->count == list->capacity)
    {
        list->capacity += PTR_LIST_DEFAULT_CAPACITY;
        list->ptr = BalReAlloc(list->ptr, SIZING(list->capacity));
    }
    (list->ptr)[list->count] = ptr;
    list->count++;
}

Long PListFirst(const restrict PPointerList list, const restrict pvoid ptr)
{
    const ULong _len = list->count;
    const restrict PVoid* _ptr = list->ptr;
    if (_len == 0)
        return -1;
    for (int x = 0; x < _len; x++)
    {
        if (_ptr[x] == ptr)
            return x;
    }
    return -1;
}

void PListForEach(const PPointerList list, const PointerListForEach forEach)
{
    const ULong _len = list->count;
    const restrict PVoid* _ptr = list->ptr;
    if (_len == 0)
        return;
    Int _res;
    for (int x = 0; x < _len; x++)
    {
        _res = forEach(_ptr[x]);
        if (_res)
            break;
    }
}

void PListRemoveAt(restrict PPointerList list, const ULong index)
{
    ULong _i = list->count - index - 1;
    const PVoid* _ptr = list->ptr + SIZING(index);
    BStringCopyD((PByte)(_ptr), (PByte)(_ptr - POINTER_SIZE), SIZING(_i));
    if (list->count == list->capacity - PTR_LIST_DEFAULT_CAPACITY - 2)
    {
        list->capacity -= 6;
        list->ptr = BalReAlloc(list->ptr, SIZING(list->capacity));
    }
    list->count--;
}

void PListRemoveAtA(restrict PPointerList list, const ULong index)
{
    ULong _i = list->count - index - 1;
    const PVoid* _ptr = list->ptr + SIZING(index);
    BalFree(list->ptr[index]);
    BStringCopyD((PByte)(_ptr), (PByte)(_ptr - POINTER_SIZE), SIZING(_i));
    if (list->count == list->capacity - PTR_LIST_DEFAULT_CAPACITY - 2)
    {
        list->capacity -= 6;
        list->ptr = BalReAlloc(list->ptr, SIZING(list->capacity));
    }
    list->count--;
}

ULong PListSize(const PPointerList list)
{
    return list->count;
}

ULong PListCapacity(const PPointerList list)
{
    return list->capacity;
}

pvoid PListGetIndex(const restrict PPointerList list, const ULong index)
{
    return list->ptr[index];
}

void PListSetIndex(restrict PPointerList list, const ULong index, const restrict pvoid ptr)
{
    list->ptr[index] = ptr;
}

void PListDestroy(const PPointerList ptr)
{
    BalFree(ptr->ptr);
    BalFree(ptr);
}

void PListDestroyAndClear(const PPointerList ptr)
{
    const ULong _c = ptr->count;
    const PVoid* _ptr = ptr->ptr;
    for (ULong x = 0; x < _c; x++)
    {
        BalFree(_ptr[x]);
    }
    BalFree(ptr->ptr);
    BalFree(ptr);
}