#include <bal_string.h>
#include <stdio.h>

Int MyForEach(PBString str)
{
    BStringAppendA(str, '\0');
    printf("%s | string info {ptr: 0x%p, len: %i}\n", str->ptr, str->ptr, str->len);
    BStringDestroy(str);
    return 0;
}

int main()
{
    PChar _char = (pchar)"test;abi";
    PBString str = BStringCreateA(_char);
    PPointerList list = BStringSplit(str, ';');
    if (list)
    {
        PListForEach(list, (PointerListForEach)MyForEach);
        PListDestroy(list);
    }
    BStringDestroy(str);
}