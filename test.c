#include <bal_string.h>
#include <bal_memory.h>
#include <stdio.h>
#define LENGTH 10

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

    PChar _char = (PChar)"a a a atest;t t t  aaaaaaaasssssssssdddddddd";

    BString _str = BStringCreateLocal(_char);

    PBString __str = BStringClone(&_str);

    PBString str = BStringCreateA(_char);
    BStringTrim(str);
    PPointerList list = BStringSplit(str, ';');
    
    if (list)
    {
        PListForEach(list, (PointerListForEach)MyForEach);
        PListDestroy(list);
    }
    BStringDestroy(str);
    BStringDestroy(__str);
}
