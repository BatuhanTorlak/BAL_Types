#include <bal_string.h>
#include <bal_memory.h>
#include <stdio.h>

Int MyForEach(PBString str)
{
    BStringAppendA(str, '\0');
    printf("%s | string info {ptr: 0x%p, len: %i}\n", str->ptr, str->ptr, str->len);
    BStringDestroy(str);
    return 0;
}

Int main(const PPointerList args)
{
    if (args != 0)
        printf("test count: %i\n", args->count);
    PChar _char = (PChar)"    test;t t t  ";
    PBString str = BStringCreateA(_char);
    printf("%s\n", str->ptr);
    BStringTrim(str);
    printf("%s\n", str->ptr);
    PPointerList list = BStringSplit(str, ';');
    printf("0x%p\n", list);
    if (list)
    {
        PListForEach(list, (PointerListForEach)MyForEach);
        PListDestroy(list);
    }
    BStringDestroy(str);
}
