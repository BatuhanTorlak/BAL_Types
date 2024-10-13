#include <bal_types.h>
#include <bal_string.h>
#include <bal_pointer_list.h>
#include <bal_defs.h>
#include <windows.h>
#include <stdio.h>

void LOCAL_FUNCTION LF_BalEntry();
void LOCAL_FUNCTION LF_BalExit();
void BalMemoryInit();
void BalMemoryDeInit();
Int main(const PPointerList args);

INT BalEntry(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPCSTR     lpCmdLine, 
    INT       nShowCmd
) {
    LF_BalEntry();
    BString _argString = BStringCreateLocal((PChar)lpCmdLine);
    printf("0x%p 0x%p 0x%p 0x%p\n", hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    for (int x = 0; x < 10; x++)
    {
        printf("%c", lpCmdLine[x]);
    }
    printf("\n");
    PPointerList _argList = BStringSplit(&_argString, ' ');
    Int _res = main(_argList);
    if (_argList)
    {
        const ULong _len = PListSize(_argList);
        for (ULong x = 0; x < _len; x++)
        {
            const PBString _str = PListGetIndex(_argList, x);
            BStringDestroy(_str);
        }
        PListDestroy(_argList);
    }
    LF_BalExit();
    return (INT)_res;
}

void LOCAL_FUNCTION LF_BalEntry()
{
    BalMemoryInit();
}

void LOCAL_FUNCTION LF_BalExit()
{
    BalMemoryDeInit();
}