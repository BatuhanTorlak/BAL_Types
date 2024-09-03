#include <malloc.h>
#include <string.h>
#include "bal_pointer_list.h"
#define PTR_LIST_DEFAULT_CAPACITY 6
#define SIZING(x) (sizeof(pvoid) * (x))

PPointerList PListCreate(Int capacity)
{
    if (capacity == 0)
        capacity = PTR_LIST_DEFAULT_CAPACITY;
    PPointerList _p = malloc(sizeof(PointerList));
    _p->capacity = capacity;
    _p->ptr = malloc(SIZING(capacity));
    return _p;
}

void PListAdd(PPointerList list, pvoid ptr)
{
    if (list->count == list->capacity)
    {
        list->capacity += PTR_LIST_DEFAULT_CAPACITY;
        list->ptr = realloc(list->ptr, SIZING(list->capacity));
    }
    (list->ptr)[list->count] = ptr;
    list->count++;
}

void PListRemoveAt(PPointerList list, Int index)
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

void PListRemoveAtA(PPointerList list, Int index)
{
    if (list->count == list->capacity - PTR_LIST_DEFAULT_CAPACITY - 2)
    {
        list->capacity -= 6;
        list->ptr = realloc(list->ptr, SIZING(list->capacity));
    }
    free(list->ptr + SIZING(index));
    int _i = list->count - index - 1;
    memcpy(list->ptr, list->ptr + POINTER_SIZE, SIZING(_i));
    list->count--;
}

pvoid PListGetIndex(PPointerList list, Int index)
{
    return list->ptr[index];
}

void PListSetIndex(PPointerList list, Int index, pvoid ptr)
{
    list->ptr[index] = ptr;
}

void PListDestroy(PPointerList ptr)
{
    free(ptr->ptr);
    free(ptr);
}

void PListDestroyAndClear(PPointerList ptr)
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