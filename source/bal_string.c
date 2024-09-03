#include "bal_string.h"
#include <malloc.h>
#include <memory.h>
#define BSTRING_DEFAULT_CAPACITY 8
#define BSTRING_DEFAULT_CAPACITY_ADD 6
#define LOCAL_FUNCTION static
#define BSTRING_SIZE sizeof(BString)

Bool LOCAL_FUNCTION LF_StrCmpr(const restrict PChar first, const restrict PChar second, const ULong length);
void LOCAL_FUNCTION LF_UnsafeBStringAppend(restrict PBString str, restrict PBString apnd);
ULong LOCAL_FUNCTION LF_StrLength(const restrict PChar cstr);

PBString BStringCreate()
{
    return BStringCreateD(BSTRING_DEFAULT_CAPACITY);
}

PBString BStringCreateA(const restrict PChar ptr)
{
    const ULong _len = LF_StrLength(ptr);

    PBString _str = malloc(BSTRING_SIZE);
    _str->ptr = malloc(CHAR_SIZE * _len);

    memmove(_str->ptr, ptr, CHAR_SIZE * _len);

    _str->len = _len;
    _str->capacity = _len;

    return _str;
}

PBString BStringCreateB(const restrict PChar ptr, const ULong length)
{
    PBString _str = malloc(BSTRING_SIZE);
    _str->ptr = malloc(CHAR_SIZE * length);

    memmove(_str->ptr, ptr, CHAR_SIZE * length);

    _str->len = length;
    _str->capacity = length;
    
    return _str;
}

PBString BStringCreateC(const restrict PChar ptr, const ULong length, const ULong capacity)
{
    PBString _str = malloc(BSTRING_SIZE);
    if (capacity < length)
        return 0;

    _str->ptr = malloc(CHAR_SIZE * capacity);

    memmove(_str->ptr, ptr, CHAR_SIZE * length);

    _str->len = length;
    _str->capacity = capacity;
}

PBString BStringCreateD(const ULong capacity)
{
    PBString _str = malloc(BSTRING_SIZE);
    
    _str->ptr = malloc(CHAR_SIZE * capacity);

    _str->len = 0;
    _str->capacity = capacity;

    return _str;
}

BString BStringCreateLocal(const restrict PChar ptr)
{
    ULong _len = 0;

    while (ptr[_len] != 0) _len++;

    const BString _str = {
        .ptr = ptr,
        .len = _len,
        .capacity = 0
    };

    return _str;
}

BString BStringCreateLocalA(const restrict PChar ptr, const ULong size)
{
    const BString _str = {
        .ptr = ptr,
        .len = size,
        .capacity = 0
    };

    return _str;
}

void BStringResetCapacity(restrict PBString str, ULong newCapacity)
{
    if (newCapacity <= str->len)
        newCapacity = str->len;
    str->ptr = realloc(str->ptr, newCapacity * CHAR_SIZE);
    str->capacity = newCapacity;
}

ULong BStringLength(const PBString str)
{
    return str->len;
}

Bool BStringStartsWith(const restrict PBString str, const restrict PBString cmpr)
{
    if (str->len < cmpr->len)
        return 0;
    return LF_StrCmpr(str->ptr, cmpr->ptr, cmpr->len);
}

Bool BStringStartsWithA(const restrict PBString str, const Char word)
{
    if (str->len == 0)
        return 0;
    return str->ptr[0] == word;
}

Bool BStringEndsWith(const restrict PBString str, const restrict PBString cmpr)
{
    if (str->len < cmpr->len)
        return 0;
    return LF_StrCmpr(str->ptr + CHAR_SIZE * (str->len - cmpr->len), cmpr->ptr, cmpr->len);
}

Bool BStringEndsWithA(const restrict PBString str, const Char word)
{
    if (str->len == 0)
        return 0;
    return str->ptr[str->len - 1] == word;
}

Bool BStringCompare(const restrict PBString str, const restrict PBString cmpr)
{
    const ULong _len = str->len;
    if (_len != cmpr->len)
        return 0;
    if (_len == 0)
        return 1;
    return LF_StrCmpr(str->ptr, cmpr->ptr, _len);
}

Bool BStringContains(const restrict PBString str, const restrict PBString cmpr)
{
    const ULong _len = cmpr->len;
    ULong _sLen = str->len;
    if ((_len | _sLen) == 0 || _len > _sLen)
        return 0;
    if (_len == _sLen)
        return LF_StrCmpr(str->ptr, cmpr->ptr, _len);
    _sLen -= _len;
    for (int x = 0; x < _sLen; x++)
    {
        for (int y = 0; y < _len; y++)
        {
            if (str->ptr[x] != cmpr->ptr[y])
            {
                goto _e;
            }
        }
        return 1;
        _e:;
    }
    return 0;
}

Bool BStringContainsA(const restrict PBString str, const Char word)
{
    const ULong _len = str->len;
    const Char* _ptr = str->ptr;
    if (_len == 0)
        return 0;
    for (int x = 0; x < _len; x++)
    {
        if (_ptr[x] == word)
            return 1;
    }
    return 0;
}

PBString BStringConcat(const PBString first, const PBString second)
{
    const ULong _fLen = first->len;
    const ULong _sLen = second->len;
    const Char* _ptr = malloc((_fLen + _sLen) * CHAR_SIZE);
    memmove(_ptr, first->ptr, _fLen * CHAR_SIZE);
    memmove(_ptr + _fLen * CHAR_SIZE, second->ptr, _sLen * CHAR_SIZE);
    return PBStringCreateB(_ptr, _fLen + _sLen);
}

PBString BStringConcatA(const restrict PBString list, const ULong count)
{
    if (count == 0)
        return 0;
    if (count == 1)
        return BStringClone(list);
    if (count == 2)
        return BStringConcat(list, list + BSTRING_SIZE);
    PBString _s = BStringConcat(list, list + BSTRING_SIZE);
    ULong _len = _s->len;
    for (int x = 2; x < count; x++)
    {
        _len += _s[x].len;
    }
    BStringResetCapacity(_s, _len);
    for (int x = 2; x < count; x++)
    {
        LF_UnsafeBStringAppend(_s, list + BSTRING_SIZE * x);
    }
    return 0;
}

void BStringAppend(restrict PBString str, const restrict PBString apnd)
{
    if (str->capacity - str->len >= apnd->len)
    {
        memmove(str->ptr + CHAR_SIZE * str->len, apnd->ptr, apnd->len * CHAR_SIZE);
        str->len += apnd->len;
        return;
    }
    str->capacity = str->len + apnd->len + BSTRING_DEFAULT_CAPACITY_ADD;
    str->ptr = realloc(str->ptr, str->capacity * CHAR_SIZE);
    memmove(str->ptr + CHAR_SIZE * str->len, apnd->ptr, apnd->len * CHAR_SIZE);
    str->len = str->capacity - BSTRING_DEFAULT_CAPACITY_ADD;
}

void BStringAppendA(restrict PBString str, Char apnd)
{
    if (str->capacity - str->len >= CHAR_SIZE)
    {
        str->ptr[str->len] = apnd;
        str->len++;
        return;
    }
    str->capacity += BSTRING_DEFAULT_CAPACITY_ADD;
    str->ptr = realloc(str->ptr, str->capacity * CHAR_SIZE);
    str->ptr[str->len] = apnd;
    str->len++;
}

void BStringAppendB(restrict PBString str, restrict PChar apnd)
{
    ULong _len = LF_StrLength(apnd);
    if (str->capacity - str->len >= _len)
    {
        memmove(str->ptr + CHAR_SIZE * str->len, apnd, _len * CHAR_SIZE);
        str->len += _len;
        return;
    }
    str->capacity = str->len + _len + BSTRING_DEFAULT_CAPACITY_ADD;
    str->ptr = realloc(str->ptr, str->capacity * CHAR_SIZE);
    memmove(str->ptr + CHAR_SIZE * str->len, apnd, _len * CHAR_SIZE);
    str->len = str->capacity - BSTRING_DEFAULT_CAPACITY_ADD;
}

void BStringClear(restrict PBString str)
{
    str->capacity = BSTRING_DEFAULT_CAPACITY;
    str->ptr = realloc(str->ptr, BSTRING_DEFAULT_CAPACITY * CHAR_SIZE);
    str->len = 0;
}

PBString BStringClone(const restrict PBString str)
{
    PBString _str = malloc(sizeof(BString));
    _str->ptr = malloc(str->capacity * CHAR_SIZE);
    _str->capacity = str->capacity;
    _str->len = str->len;
    memmove(_str->ptr, str->ptr, _str->len);
    return _str;
}

void BStringDestroy(const restrict PBString str)
{
    free(str->ptr);
    free(str);
}

Bool LOCAL_FUNCTION LF_StrCmpr(const restrict PChar first, const restrict PChar second, const ULong length)
{
    const ULong _a = (length * CHAR_SIZE) / sizeof(ULong);
    const ULong _b = (length * CHAR_SIZE) % sizeof(ULong) / CHAR_SIZE;
    for (int x = 0; x < _a; x++)
    {
        if (((ULong*)first)[x] != ((ULong*)second)[x])
            return 0;
    }
    for (int x = length - _b; x < length; x++)
    {
        if (first[x] != second[x])
            return 0;
    }
    return 1;
}

void LOCAL_FUNCTION LF_UnsafeBStringAppend(restrict PBString str, restrict PBString apnd)
{
    memmove(str->ptr + str->len * CHAR_SIZE, apnd->ptr, apnd->len * CHAR_SIZE);
    str->len += apnd->len;
}

ULong LOCAL_FUNCTION inline LF_StrLength(const restrict register PChar cstr)
{
    register ULong _len = 0;
    while (cstr[_len++]);
    return _len;
}