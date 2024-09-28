#include "bal_string16.h"
#include "bal_defs.h"
#include <bal_memory.h>
#undef CHAR_SIZE
#define CHAR_SIZE sizeof(Char16)
#define BSTRING_DEFAULT_CAPACITY 8
#define BSTRING_DEFAULT_CAPACITY_ADD 6
#define BSTRING_SIZE sizeof(BString16)
#define CHECK_IS_NOT_EMPTY_WORD(x) ((x) != ' ' && (x) != '\t' && (x) != '\n' && (x) != '\r' && (x) != '\0')
#define CHECK_IS_EMPTY_WORD(x) ((x) == ' ' || (x) == '\t' || (x) == '\n' || (x) == '\r' || (x) == '\0')

Bool LOCAL_FUNCTION LF_StrCmpr(const restrict PChar16 first, const restrict PChar16 second, const ULong length);
Bool LOCAL_FUNCTION LF_StrCmprA(const restrict PChar16 first, const restrict PChar16 second, const ULong len, const ULong len1, const ULong len2);
Bool LOCAL_FUNCTION LF_IsEmptyArea(const restrict PChar16 str, ULong start, const ULong end);
void LOCAL_FUNCTION LF_UnsafeBStringAppend(restrict PBString16 str, restrict PBString16 apnd);
ULong LOCAL_FUNCTION LF_StrLength(const restrict PChar16 cstr);

PBString16 LOCAL_FUNCTION _BString16CreateA(const restrict PChar16 ptr)
{
    const ULong _len = LF_StrLength(ptr);

    PBString16 _str = BalAlloc(BSTRING_SIZE);
    _str->ptr = ptr;

    _str->len = _len;
    _str->capacity = _len;

    return _str;
}

PBString16 LOCAL_FUNCTION _BString16CreateB(const restrict PChar16 ptr, const ULong length)
{
    PBString16 _str = BalAlloc(BSTRING_SIZE);
    _str->ptr = ptr;

    _str->len = length;
    _str->capacity = length;
    
    return _str;
}

PBString16 BString16Create()
{
    return BString16CreateD(BSTRING_DEFAULT_CAPACITY);
}

PBString16 BString16CreateA(const restrict PChar16 ptr)
{
    const ULong _len = LF_StrLength(ptr);

    PBString16 _str = BalAlloc(BSTRING_SIZE);
    _str->ptr = BalAlloc(CHAR_SIZE * _len);

    BString16CopyD(ptr, _str->ptr, CHAR_SIZE * _len);

    _str->len = _len;
    _str->capacity = _len;

    return _str;
}

PBString16 BString16CreateB(const restrict PChar16 ptr, const ULong length)
{
    PBString16 _str = BalAlloc(BSTRING_SIZE);
    _str->ptr = BalAlloc(CHAR_SIZE * length);

    BString16CopyD(ptr, _str->ptr, CHAR_SIZE * length);

    _str->len = length;
    _str->capacity = length;
    
    return _str;
}

PBString16 BString16CreateC(const restrict PChar16 ptr, const ULong length, const ULong capacity)
{
    PBString16 _str = BalAlloc(BSTRING_SIZE);
    if (capacity < length)
        return 0;

    _str->ptr = BalAlloc(CHAR_SIZE * capacity);

    BString16CopyD(ptr, _str->ptr, CHAR_SIZE * length);

    _str->len = length;
    _str->capacity = capacity;
}

PBString16 BString16CreateD(const ULong capacity)
{
    PBString16 _str = BalAlloc(BSTRING_SIZE);
    
    _str->ptr = BalAlloc(CHAR_SIZE * capacity);

    _str->len = 0;
    _str->capacity = capacity;

    return _str;
}

BString16 BString16CreateLocal(const restrict PChar16 ptr)
{
    ULong _len = 0;

    while (ptr[_len] != 0) _len++;

    const BString16 _str = {
        .ptr = ptr,
        .len = _len,
        .capacity = 0
    };

    return _str;
}

BString16 BString16CreateLocalA(const restrict PChar16 ptr, const ULong size)
{
    const BString16 _str = {
        .ptr = ptr,
        .len = size,
        .capacity = 0
    };

    return _str;
}

void BString16ResetCapacity(restrict PBString16 str, ULong newCapacity)
{
    if (newCapacity <= str->len)
        newCapacity = str->len;
    str->ptr = BalReAlloc(str->ptr, newCapacity * CHAR_SIZE);
    str->capacity = newCapacity;
}

ULong BString16Length(const PBString16 str)
{
    return str->len;
}

Char16 BString16Get(const restrict PBString16 str, const ULong index)
{
    return str->ptr[index];
}

void BString16Set(restrict PBString16 str, const ULong index, Char16 chr)
{
    str->ptr[index] = chr;
}

Bool BString16StartsWith(const restrict PBString16 str, const restrict PBString16 cmpr)
{
    if (str->len < cmpr->len)
        return 0;
    return LF_StrCmpr(str->ptr, cmpr->ptr, cmpr->len);
}

Bool BString16StartsWithA(const restrict PBString16 str, const Char16 word)
{
    if (str->len == 0)
        return 0;
    return str->ptr[0] == word;
}

Bool BString16EndsWith(const restrict PBString16 str, const restrict PBString16 cmpr)
{
    if (str->len < cmpr->len)
        return 0;
    return LF_StrCmpr(str->ptr + CHAR_SIZE * (str->len - cmpr->len), cmpr->ptr, cmpr->len);
}

Bool BString16EndsWithA(const restrict PBString16 str, const Char16 word)
{
    if (str->len == 0)
        return 0;
    return str->ptr[str->len - 1] == word;
}

Bool BString16Compare(const restrict PBString16 str, const restrict PBString16 cmpr)
{
    const ULong _len = str->len;
    if (_len != cmpr->len)
        return 0;
    if (_len == 0)
        return 1;
    return LF_StrCmpr(str->ptr, cmpr->ptr, _len);
}

Bool BString16Contains(const restrict PBString16 str, const restrict PBString16 cmpr)
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

    const restrict PChar16 _ptr1 = str->ptr;
    const restrict PChar16 _ptr2 = cmpr->ptr;
    for (int x = 0; x < _sLen; x++)
    {
        if (LF_StrCmprA(_ptr1 + CHAR_SIZE * x, _ptr2, _len, _a, _b))
            return 1;
    }
    return 0;
}

Bool BString16ContainsA(const restrict PBString16 str, const Char16 word)
{
    const ULong _len = str->len;
    const Char16* _ptr = str->ptr;
    if (_len == 0)
        return 0;
    for (int x = 0; x < _len; x++)
    {
        if (_ptr[x] == word)
            return 1;
    }
    return 0;
}

Bool BString16IsEmpty(const restrict PBString16 str)
{
    const restrict PChar16 _ptr = str->ptr;
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

PBString16 BString16Concat(const PBString16 first, const PBString16 second)
{
    const ULong _fLen = first->len;
    const ULong _sLen = second->len;
    const Char16* _ptr = BalAlloc((_fLen + _sLen) * CHAR_SIZE);
    BString16CopyD(_ptr, first->ptr, _fLen * CHAR_SIZE);
    BString16CopyD(_ptr + _fLen * CHAR_SIZE, second->ptr, _sLen * CHAR_SIZE);
    return _BString16CreateB(_ptr, _fLen + _sLen);
}

PBString16 BString16ConcatA(const restrict PBString16 list, const ULong count)
{
    if (count == 0)
        return 0;
    if (count == 1)
        return BString16Clone(list);
    if (count == 2)
        return BString16Concat(list, list + BSTRING_SIZE);
    PBString16 _s = BString16Concat(list, list + BSTRING_SIZE);
    ULong _len = _s->len;
    for (int x = 2; x < count; x++)
    {
        _len += list[x].len;
    }
    BString16ResetCapacity(_s, _len);
    for (int x = 2; x < count; x++)
    {
        LF_UnsafeBStringAppend(_s, list + BSTRING_SIZE * x);
    }
    return 0;
}

PBString16 BString16Substring(const PBString16 str, const ULong start, const ULong length)
{
    if (length <= 0 || start >= str->len || start < 0)
        return 0;
    return BString16CreateB(str->ptr + start * CHAR_SIZE, length);
}

PBString16 BString16SubstringA(const PBString16 str, const ULong start)
{
    if (start >= str->len || start < 0)
        return 0;
    return BString16CreateB(str->ptr + start * CHAR_SIZE, str->len - start);
}

PBString16 BString16SubstringB(const PBString16 str, const ULong end)
{
    if (end < 0 || end >= str->len)
        return 0;
    return BString16CreateB(str->ptr, end);
}

PBString16 BString16SubstringC(const PBString16 str, const ULong start, const ULong end)
{
    if (end < start || end >= str->len || start < 0)
        return 0;
    return BString16CreateB(str->ptr + start * CHAR_SIZE, end - start + 1);
}

Long BString16First(const restrict PBString16 str, const restrict PBString16 frt)
{
    ULong _uLen = frt->len;
    ULong _len = str->len;
    const restrict PChar16 _ptr1 = str->ptr;
    const restrict PChar16 _ptr2 = frt->ptr;

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

Long BString16FirstA(const restrict PBString16 str, const Char16 chr)
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

void BString16Trim(const restrict PBString16 str)
{
    ULong _len = str->len;
    ULong _trimStart = 0;
    ULong _trimEnd = 0;
    const restrict PChar16 _ptr = str->ptr;
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
    restrict PChar16 _char = BalAlloc(CHAR_SIZE * str->capacity);
    BString16CopyD(_ptr + _trimStart * CHAR_SIZE, _ptr, _len * CHAR_SIZE);
    str->len = _len;
}

void BString16TrimStart(const restrict PBString16 str)
{
    ULong _len = str->len;
    ULong _start = 0;
    const restrict PChar16 _ptr = str->ptr;
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
    BString16CopyD(_ptr + _start * CHAR_SIZE, _ptr, _len * CHAR_SIZE);
    str->len = _len;
}

void BString16TrimEnd(const restrict PBString16 str)
{
    const restrict PChar16 _ptr = str->ptr;
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

void BString16Replace(const restrict PBString16 str, const Char16 replaced, const Char16 replacing)
{
    restrict PChar16 _ptr = str->ptr;
    const ULong _len = str->len;
    for (int x = 0; x < _len; x++)
    {
        if (_ptr[x] == replaced)
            _ptr[x] = replacing;
    }
}

void BString16Append(restrict PBString16 str, const restrict PBString16 apnd)
{
    if (str->capacity == 0)
        return;
    if (str->capacity - str->len >= apnd->len)
    {
        BString16CopyD(apnd->ptr, str->ptr + CHAR_SIZE * str->len, apnd->len * CHAR_SIZE);
        str->len += apnd->len;
        return;
    }
    str->capacity = str->len + apnd->len + BSTRING_DEFAULT_CAPACITY_ADD;
    str->ptr = BalReAlloc(str->ptr, str->capacity * CHAR_SIZE);
    BString16CopyD(apnd->ptr, str->ptr + CHAR_SIZE * str->len, apnd->len * CHAR_SIZE);
    str->len = str->capacity - BSTRING_DEFAULT_CAPACITY_ADD;
}

void BString16AppendA(restrict PBString16 str, Char16 apnd)
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

void BString16AppendB(restrict PBString16 str, restrict PChar16 apnd)
{
    if (str->capacity == 0)
        return;
    ULong _len = LF_StrLength(apnd);
    if (str->capacity - str->len >= _len)
    {
        BString16CopyD(apnd, str->ptr + CHAR_SIZE * str->len, _len * CHAR_SIZE);
        str->len += _len;
        return;
    }
    str->capacity = str->len + _len + BSTRING_DEFAULT_CAPACITY_ADD;
    str->ptr = BalReAlloc(str->ptr, str->capacity * CHAR_SIZE);
    BString16CopyD(apnd, str->ptr + CHAR_SIZE * str->len, _len * CHAR_SIZE);
    str->len = str->capacity - BSTRING_DEFAULT_CAPACITY_ADD;
}

void BString16AppendC(restrict PBString16 str, restrict PChar16 apnd, const ULong length)
{
    if (str->capacity == 0)
        return;
    if (str->capacity - str->len >= length)
    {
        BString16CopyD(apnd, str->ptr + CHAR_SIZE * str->len, length * CHAR_SIZE);
        str->len += length;
        return;
    }
    str->capacity = str->len + length + BSTRING_DEFAULT_CAPACITY_ADD;
    str->ptr = BalReAlloc(str->ptr, str->capacity * CHAR_SIZE);
    BString16CopyD(apnd, str->ptr + CHAR_SIZE * str->len, length * CHAR_SIZE);
    str->len = str->capacity - BSTRING_DEFAULT_CAPACITY_ADD;
}

void BString16Clear(restrict PBString16 str)
{
    if (str->capacity == 0)
        return;
    str->capacity = BSTRING_DEFAULT_CAPACITY;
    str->ptr = BalReAlloc(str->ptr, BSTRING_DEFAULT_CAPACITY * CHAR_SIZE);
    str->len = 0;
}

void BString16Copy(const PBString16 from, const PBString16 to)
{
    if (from->len < to->len)
        return;
    BString16CopyD(from->ptr, to->ptr, to->len);
}

void BString16CopyA(const PBString16 from, const PBString16 to, const ULong length)
{
    if (length <= 0 || to->len < length || from->len < to->len)
        return;
    BString16CopyD(from->ptr, to->ptr, length);
}

void BString16CopyB(const PBString16 from, const PBString16 to, const ULong startFrom, const ULong startTo, const ULong length)
{
    if (startFrom <= 0 || startTo <= 0 || length <= 0 || to->len < (length + startTo) || from->len < (length + startFrom))
        return;
    BString16CopyD(from->ptr + startFrom, to->ptr + startTo, length);
}

void BString16CopyC(const PShort from, restrict PShort to, const ULong length)
{
    if (length <= 0)
        return;
    for (int x = 0; x < length; x++)
    {
        to[x] = from[x];
    }
}

void BString16CopyD(const PShort from, restrict PShort to, const ULong length)
{
    if (length <= 0)
        return;
    if (length < 32)
    {
        BString16CopyC(from, to, length);
        return;
    }
    BalMemMove(from, to, length);
    return;
    const ULong _64 = length / 8;
    const ULong _8 = length % 8;
    const PQuad _ptrF = (PQuad)from;
    restrict PQuad _ptrT = (PQuad)to;
    for (ULong x = 0; x < _64; x++)
    {
        _ptrT[x] = _ptrF[x];
    }
    for (ULong x = 1; x <= _8; x++)
    {
        to[length - x] = from[length - x];
    }
}

PPointerList BString16Split(const restrict PBString16 str, const Char16 wrd)
{
    if (str->len == 0)
        return 0;
    const restrict PPointerList ptrList = PListCreate(6);
    const restrict PChar16 _ptr = str->ptr;
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
            const restrict PBString16 _str = BString16SubstringC(str, _start, x - 1);
            PListAdd(ptrList, _str);
            _start = x + 1;
        }
    }
    if (!LF_IsEmptyArea(_ptr, _start, _len))
    {
        const restrict PBString16 _str = BString16SubstringA(str, _start);
        PListAdd(ptrList, _str);
    }
    if (ptrList->count > 1)
    {
        return ptrList;
    }
    PListDestroy(ptrList);
    return 0;
}

PBString16 BString16Clone(const restrict PBString16 str)
{
    PBString16 _str = BalAlloc(BSTRING_SIZE);
    _str->ptr = BalAlloc((str->len + BSTRING_DEFAULT_CAPACITY_ADD) * CHAR_SIZE);
    _str->capacity = str->len + BSTRING_DEFAULT_CAPACITY_ADD;
    _str->len = str->len;
    BString16CopyD(str->ptr, _str->ptr, _str->len);
    return _str;
}

void BString16Destroy(const restrict PBString16 str)
{
    if (str->capacity == 0)
        return;
    BalFree(str->ptr);
    BalFree(str);
}

Bool LOCAL_FUNCTION LF_StrCmpr(const restrict PChar16 first, const restrict PChar16 second, const ULong length)
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

Bool LOCAL_FUNCTION LF_StrCmprA(const restrict PChar16 first, const restrict PChar16 second, const ULong len, const ULong len1, const ULong len2)
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

Bool LOCAL_FUNCTION LF_IsEmptyArea(const restrict PChar16 str, ULong start, const ULong end)
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

void LOCAL_FUNCTION LF_UnsafeBStringAppend(restrict PBString16 str, restrict PBString16 apnd)
{
    BString16CopyD(str->ptr + str->len * CHAR_SIZE, apnd->ptr, apnd->len * CHAR_SIZE);
    str->len += apnd->len;
}

ULong LOCAL_FUNCTION inline LF_StrLength(const restrict register PChar16 cstr)
{
    register ULong _len = 0;
    while (cstr[_len]) _len++;
    return _len;
}