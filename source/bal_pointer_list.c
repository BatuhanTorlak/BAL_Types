#include <malloc.h>
#include <string.h>
#include "bal_pointer_list.h"
#define PTR_LIST_DEFAULT_CAPACITY 6
#define SIZING(x) (sizeof(pvoid) * (x))

PPointerList PListCreate(ULong capacity)
{
    if (capacity == 0)
        capacity = PTR_LIST_DEFAULT_CAPACITY;
    PPointerList _p = malloc(sizeof(PointerList));
    _p->capacity = capacity;
    _p->count = 0;
    _p->ptr = malloc(SIZING(capacity));
    return _p;
}

void PListAdd(restrict PPointerList list, const restrict pvoid ptr)
{
    if (list->count == list->capacity)
    {
        list->capacity += PTR_LIST_DEFAULT_CAPACITY;
        list->ptr = realloc(list->ptr, SIZING(list->capacity));
    }
    (list->ptr)[list->count] = ptr;
    list->count++;
}

Long PListFirst(const restrict PPointerList list, const restrict pvoid ptr)
{
    const ULong _len = list->count;
    const restrict pvoid* _ptr = list->ptr;
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
    const restrict pvoid* _ptr = list->ptr;
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
    if (list->count == list->capacity - PTR_LIST_DEFAULT_CAPACITY - 2)
    {
        list->capacity -= 6;
        list->ptr = realloc(list->ptr, SIZING(list->capacity));
    }
    int _i = list->count - index - 1;
    memcpy(list->ptr, list->ptr + POINTER_SIZE, SIZING(_i));
    list->count--;
}

void PListRemoveAtA(restrict PPointerList list, const ULong index)
{
    if (list->count == list->capacity - PTR_LIST_DEFAULT_CAPACITY - 2)
    {
        list->capacity -= 6;
        list->ptr = realloc(list->ptr, SIZING(list->capacity));
    }
    free(list->ptr[index]);
    int _i = list->count - index - 1;
    memcpy(list->ptr, list->ptr + POINTER_SIZE, SIZING(_i));
    list->count--;
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
    free(ptr->ptr);
    free(ptr);
}

void PListDestroyAndClear(const PPointerList ptr)
{
    int _c = ptr->count;
    pvoid* _ptr = ptr->ptr;
    for (int x = 0; x < _c; x++)
    {
        free(_ptr[x]);
    }
    free(ptr->ptr);
    free(ptr);
}