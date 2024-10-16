#include "bal_string.h"
#include "bal_defs.h"
#include <bal_memory.h>
#define BSTRING_DEFAULT_CAPACITY 8
#define BSTRING_DEFAULT_CAPACITY_ADD 6
#define BSTRING_SIZE sizeof(BString)
#define CHECK_IS_NOT_EMPTY_WORD(x) ((x) != ' ' && (x) != '\t' && (x) != '\n' && (x) != '\r' && (x) != '\0')
#define CHECK_IS_EMPTY_WORD(x) ((x) == ' ' || (x) == '\t' || (x) == '\n' || (x) == '\r' || (x) == '\0')

Bool LOCAL_FUNCTION LF_StrCmpr(const restrict PChar first, const restrict PChar second, const ULong length);
Bool LOCAL_FUNCTION LF_StrCmprA(const restrict PChar first, const restrict PChar second, const ULong len, const ULong len1, const ULong len2);
Bool LOCAL_FUNCTION LF_IsEmptyArea(const restrict PChar str, ULong start, const ULong end);
void LOCAL_FUNCTION LF_UnsafeBStringAppend(restrict PBString str, restrict PBString apnd);
ULong LOCAL_FUNCTION LF_StrLength(const restrict PChar cstr);

PBString LOCAL_FUNCTION _BStringCreateA(const PChar ptr)
{
    const ULong _len = LF_StrLength(ptr);

    PBString _str = BalAlloc(BSTRING_SIZE);
    _str->ptr = ptr;

    _str->len = _len;
    _str->capacity = _len;

    return _str;
}

PBString LOCAL_FUNCTION _BStringCreateB(const PChar ptr, const ULong length)
{
    const ULong _len = LF_StrLength(ptr);

    PBString _str = BalAlloc(BSTRING_SIZE);
    _str->ptr = ptr;

    _str->len = length;
    _str->capacity = length;

    return _str;
}

PBString BStringCreate()
{
    return BStringCreateD(BSTRING_DEFAULT_CAPACITY);
}

PBString BStringCreateA(const restrict PChar ptr)
{
    const ULong _len = LF_StrLength(ptr);

    PBString _str = BalAlloc(BSTRING_SIZE);
    _str->ptr = BalAlloc(CHAR_SIZE * _len);

    BStringCopyD(ptr, _str->ptr, CHAR_SIZE * _len);

    _str->len = _len;
    _str->capacity = _len;

    return _str;
}

PBString BStringCreateB(const restrict PChar ptr, const ULong length)
{
    PBString _str = BalAlloc(BSTRING_SIZE);
    _str->ptr = BalAlloc(CHAR_SIZE * length);

    BStringCopyD(ptr, _str->ptr, CHAR_SIZE * length);

    _str->len = length;
    _str->capacity = length;
    
    return _str;
}

PBString BStringCreateC(const restrict PChar ptr, const ULong length, const ULong capacity)
{
    PBString _str = BalAlloc(BSTRING_SIZE);
    if (capacity < length)
        return 0;

    _str->ptr = BalAlloc(CHAR_SIZE * capacity);

    BStringCopyD(ptr, _str->ptr, CHAR_SIZE * length);

    _str->len = length;
    _str->capacity = capacity;
}

PBString BStringCreateD(const ULong capacity)
{
    PBString _str = BalAlloc(BSTRING_SIZE);
    
    _str->ptr = BalAlloc(CHAR_SIZE * capacity);

    _str->len = 0;
    _str->capacity = capacity;

    return _str;
}

BString BStringCreateLocal(const restrict PChar ptr)
{
    const BString _str = {
        .ptr = ptr,
        .len = LF_StrLength(ptr),
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
    str->ptr = BalReAlloc(str->ptr, newCapacity * CHAR_SIZE);
    str->capacity = newCapacity;
}

ULong BStringLength(const PBString str)
{
    return str->len;
}

Char BStringGet(const restrict PBString str, const ULong index)
{
    return str->ptr[index];
}

void BStringSet(restrict PBString str, const ULong index, Char chr)
{
    str->ptr[index] = chr;
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
    _sLen -= _len - 1;
    const ULong _a = (_len * CHAR_SIZE) / sizeof(ULong);
    const ULong _b = (_len * CHAR_SIZE) % sizeof(ULong) / CHAR_SIZE;

    const restrict PChar _ptr1 = str->ptr;
    const restrict PChar _ptr2 = cmpr->ptr;
    for (int x = 0; x < _sLen; x++)
    {
        if (LF_StrCmprA(_ptr1 + CHAR_SIZE * x, _ptr2, _len, _a, _b))
            return 1;
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

Bool BStringIsEmpty(const restrict PBString str)
{
    const restrict PChar _ptr = str->ptr;
    ULong _len = str->len - 1;
    if (_len == -1)
        return 1;
    for (; _len; _len--)
    {
        if (CHECK_IS_NOT_EMPTY_WORD(_ptr[_len]))
            return 0;
    }
    if (CHECK_IS_NOT_EMPTY_WORD(_ptr[0]))
        return 0;
    return 1;
}

PBString BStringConcat(const PBString first, const PBString second)
{
    const ULong _fLen = first->len;
    const ULong _sLen = second->len;
    const Char* _ptr = BalAlloc((_fLen + _sLen) * CHAR_SIZE);
    BStringCopyD(first->ptr, _ptr, _fLen * CHAR_SIZE);
    BStringCopyD(second->ptr, _ptr + _fLen * CHAR_SIZE, _sLen * CHAR_SIZE);
    return _BStringCreateB(_ptr, _fLen + _sLen);
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
        _len += list[x].len;
    }
    BStringResetCapacity(_s, _len);
    for (int x = 2; x < count; x++)
    {
        LF_UnsafeBStringAppend(_s, list + BSTRING_SIZE * x);
    }
    return 0;
}

PBString BStringSubstring(const PBString str, const ULong start, const ULong length)
{
    if (length <= 0 || start >= str->len || start < 0)
        return 0;
    return BStringCreateB(str->ptr + start * CHAR_SIZE, length);
}

PBString BStringSubstringA(const PBString str, const ULong start)
{
    if (start >= str->len || start < 0)
        return 0;
    return BStringCreateB(str->ptr + start * CHAR_SIZE, str->len - start);
}

PBString BStringSubstringB(const PBString str, const ULong end)
{
    if (end < 0 || end >= str->len)
        return 0;
    return BStringCreateB(str->ptr, end);
}

PBString BStringSubstringC(const PBString str, const ULong start, const ULong end)
{
    if (end < start || end >= str->len || start < 0)
        return 0;
    return BStringCreateB(str->ptr + start * CHAR_SIZE, end - start + 1);
}

Long BStringFirst(const restrict PBString str, const restrict PBString frt)
{
    ULong _uLen = frt->len;
    ULong _len = str->len;
    const restrict PChar _ptr1 = str->ptr;
    const restrict PChar _ptr2 = frt->ptr;

    if (_len == 0 || _uLen > _len)
        return -1;
    if (_len == _uLen)
        return LF_StrCmpr(_ptr1, _ptr2, _len) - 1;
    _len -= _uLen + 1;
    
    const ULong _a = (_uLen * CHAR_SIZE) / sizeof(ULong);
    const ULong _b = (_uLen * CHAR_SIZE) % sizeof(ULong) / CHAR_SIZE;

    for (int x = 0; x < _len; x++) 
    {
        if (LF_StrCmprA(_ptr1 + CHAR_SIZE * x, _ptr2, _uLen, _a, _b))
            return x;
    }
    return -1;
}

Long BStringFirstA(const restrict PBString str, const Char chr)
{
    ULong _len = str->len;
    if (_len == 0)
        return -1;
    for (int x = 0; x < _len; x++)
    {
        if (str->ptr[x] == chr)
            return x;
    }
    return -1;
}

void BStringTrim(const restrict PBString str)
{
    ULong _len = str->len;
    ULong _trimStart = 0;
    ULong _trimEnd = 0;
    const restrict PChar _ptr = str->ptr;
    if (_len == 0)
        return;
    for (int x = 0; x < _len; x++)
    {
        if (CHECK_IS_NOT_EMPTY_WORD(_ptr[x]))
        {
            break;
        }
        _trimStart++;
    }
    for (int x = _trimStart; x < _len; x++)
    {
        if (CHECK_IS_NOT_EMPTY_WORD(_ptr[x]))
        {
            _trimEnd = 0;
            continue;
        }
        _trimEnd++;
    }
    _len -= _trimEnd + _trimStart;
    if (!(_trimStart | _trimEnd))
        return;
    if (!_len)
    {
        str->len = 0;
        return;
    }
    BStringCopyD(_ptr + _trimStart * CHAR_SIZE, _ptr, _len * CHAR_SIZE);
    str->len = _len;
}

void BStringTrimStart(const restrict PBString str)
{
    ULong _len = str->len;
    ULong _start = 0;
    const restrict PChar _ptr = str->ptr;
    if (_len == 0)
        return;
    for (int x = 0; x < _len; x++)
    {
        if (CHECK_IS_NOT_EMPTY_WORD(_ptr[x]))
        {
            break;
        }
        _start++;
    }
    _len -= _start;
    if (!_start)
        return;
    if (!_len)
    {
        str->len = 0;
        return;
    }
    BStringCopyD(_ptr + _start * CHAR_SIZE, _ptr, _len * CHAR_SIZE);
    str->len = _len;
}

void BStringTrimEnd(const restrict PBString str)
{
    const restrict PChar _ptr = str->ptr;
    ULong _len = str->len;
    ULong _start = 0;
    if (_len == 0)
        return;
    for (int x = _len - 1; x >= 0; x--)
    {
        if (CHECK_IS_NOT_EMPTY_WORD(_ptr[x]))
        {
            break;
        }
        _start++;
    }
    _len -= _start;
    if (!_start)
        return;
    if (!_len)
    {
        str->len = 0;
        return;
    }
    str->len = _len;
}

void BStringReplace(const restrict PBString str, const Char replaced, const Char replacing)
{
    restrict PChar _ptr = str->ptr;
    const ULong _len = str->len;
    for (int x = 0; x < _len; x++)
    {
        if (_ptr[x] == replaced)
            _ptr[x] = replacing;
    }
}

void BStringAppend(restrict PBString str, const restrict PBString apnd)
{
    if (str->capacity == 0)
        return;
    if (str->capacity - str->len >= apnd->len)
    {
        BStringCopyD(apnd->ptr, str->ptr + CHAR_SIZE * str->len, apnd->len * CHAR_SIZE);
        str->len += apnd->len;
        return;
    }
    str->capacity = str->len + apnd->len + BSTRING_DEFAULT_CAPACITY_ADD;
    str->ptr = BalReAlloc(str->ptr, str->capacity * CHAR_SIZE);
    BStringCopyD(apnd->ptr, str->ptr + CHAR_SIZE * str->len, apnd->len * CHAR_SIZE);
    str->len = str->capacity - BSTRING_DEFAULT_CAPACITY_ADD;
}

void BStringAppendA(restrict PBString str, Char apnd)
{
    if (str->capacity == 0)
        return;
    if (str->capacity - str->len >= CHAR_SIZE)
    {
        str->ptr[str->len] = apnd;
        str->len++;
        return;
    }
    str->capacity += BSTRING_DEFAULT_CAPACITY_ADD;
    str->ptr = BalReAlloc(str->ptr, str->capacity * CHAR_SIZE);
    str->ptr[str->len] = apnd;
    str->len++;
}

void BStringAppendB(restrict PBString str, restrict PChar apnd)
{
    if (str->capacity == 0)
        return;
    ULong _len = LF_StrLength(apnd);
    if (str->capacity - str->len >= _len)
    {
        BStringCopyD(apnd, str->ptr + CHAR_SIZE * str->len, _len * CHAR_SIZE);
        str->len += _len;
        return;
    }
    str->capacity = str->len + _len + BSTRING_DEFAULT_CAPACITY_ADD;
    str->ptr = BalReAlloc(str->ptr, str->capacity * CHAR_SIZE);
    BStringCopyD(apnd, str->ptr + CHAR_SIZE * str->len, _len * CHAR_SIZE);
    str->len = str->capacity - BSTRING_DEFAULT_CAPACITY_ADD;
}

void BStringAppendC(restrict PBString str, restrict PChar apnd, const ULong length)
{
    if (str->capacity == 0)
        return;
    if (str->capacity - str->len >= length)
    {
        BStringCopyD(apnd, str->ptr + CHAR_SIZE * str->len, length * CHAR_SIZE);
        str->len += length;
        return;
    }
    str->capacity = str->len + length + BSTRING_DEFAULT_CAPACITY_ADD;
    str->ptr = BalReAlloc(str->ptr, str->capacity * CHAR_SIZE);
    BStringCopyD(apnd, str->ptr + CHAR_SIZE * str->len, length * CHAR_SIZE);
    str->len = str->capacity - BSTRING_DEFAULT_CAPACITY_ADD;
}

void BStringClear(restrict PBString str)
{
    if (str->capacity == 0)
        return;
    str->capacity = BSTRING_DEFAULT_CAPACITY;
    str->ptr = BalReAlloc(str->ptr, BSTRING_DEFAULT_CAPACITY * CHAR_SIZE);
    str->len = 0;
}

void BStringCopy(const PBString from, const PBString to)
{
    if (from->len < to->len)
        return;
    BStringCopyD(from->ptr, to->ptr, to->len);
}

void BStringCopyA(const PBString from, const PBString to, const ULong length)
{
    if (length <= 0 || to->len < length || from->len < to->len)
        return;
    BStringCopyD(from->ptr, to->ptr, length);
}

void BStringCopyB(const PBString from, const PBString to, const ULong startFrom, const ULong startTo, const ULong length)
{
    if (startFrom <= 0 || startTo <= 0 || length <= 0 || to->len < (length + startTo) || from->len < (length + startFrom))
        return;
    BStringCopyD(from->ptr + startFrom, to->ptr + startTo, length);
}

void BStringCopyC(const PChar from, restrict PChar to, const ULong length)
{
    if (length == 0)
        return;
    for (int x = 0; x < length; x++)
    {
        to[x] = from[x];
    }
}

void BStringCopyD(const PByte from, restrict PByte to, const ULong length)
{
    if (length == 0)
        return;
    BalMemMove(from, to, length);
    /*return;
    const ULong _64 = length / sizeof(IntVector2);
    const ULong _8 = length % sizeof(IntVector2);
    const PIntVector2 _ptrF = (PIntVector2)from;
    restrict PIntVector2 _ptrT = (PIntVector2)to;
    for (ULong x = 0; x < _64; x++)
    {
        _ptrT[x] = _ptrF[x];
    }
    for (ULong x = 1; x <= _8; x++)
    {
        to[length - x] = from[length - x];
    }*/
}

PPointerList BStringSplit(const restrict PBString str, const Char wrd)
{
    if (str->len == 0)
        return 0;
    const restrict PPointerList ptrList = PListCreate(6);
    const restrict PChar _ptr = str->ptr;
    const ULong _len = str->len;
    ULong _start = 0;
    for (int x = 0; x < _len; x++)
    {
        if (_ptr[x] == wrd)
        {
            if (LF_IsEmptyArea(_ptr, _start, x))
            {
                _start = x + 1;
                continue;
            }
            const restrict PBString _str = BStringSubstringC(str, _start, x - 1);
            PListAdd(ptrList, _str);
            _start = x + 1;
        }
    }
    if (!LF_IsEmptyArea(_ptr, _start, _len))
    {
        const restrict PBString _str = BStringSubstringA(str, _start);
        PListAdd(ptrList, _str);
    }
    if (ptrList->count > 1)
    {
        return ptrList;
    }
    PListDestroy(ptrList);
    return 0;
}

PBString BStringClone(const restrict PBString str)
{
    PBString _str = BalAlloc(sizeof(BString));
    _str->ptr = BalAlloc((str->len + BSTRING_DEFAULT_CAPACITY_ADD) * CHAR_SIZE);
    _str->capacity = str->len + BSTRING_DEFAULT_CAPACITY_ADD;
    _str->len = str->len;
    BStringCopyD(str->ptr, _str->ptr, _str->len * CHAR_SIZE);
    return _str;
}

void BStringDestroy(const restrict PBString str)
{
    if (str->capacity == 0)
        return;
    BalFree(str->ptr);
    BalFree(str);
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

Bool LOCAL_FUNCTION LF_StrCmprA(const restrict PChar first, const restrict PChar second, const ULong len, const ULong len1, const ULong len2)
{
    for (int x = 0; x < len1; x++)
    {
        if (((ULong*)first)[x] != ((ULong*)second)[x])
            return 0;
    }
    for (int x = len - len2; x < len; x++)
    {
        if (first[x] != second[x])
            return 0;
    }
    return 1;
}

Bool LOCAL_FUNCTION LF_IsEmptyArea(const restrict PChar str, ULong start, const ULong end)
{
    if (start == end)
    {
        return 1;
    }
    for (; start < end; start++)
    {
        if (CHECK_IS_NOT_EMPTY_WORD(str[start]))
        {
            return 0;
        }
    }
    return 1;
}

void LOCAL_FUNCTION LF_UnsafeBStringAppend(restrict PBString str, restrict PBString apnd)
{
    BStringCopyD(apnd->ptr, str->ptr + str->len * CHAR_SIZE, apnd->len * CHAR_SIZE);
    str->len += apnd->len;
}

ULong LOCAL_FUNCTION inline LF_StrLength(const restrict register PChar cstr)
{
    register ULong _len = 0;
    while (cstr[_len]) _len++;
    return _len;
}