#include <bal_types.h>
#include <bal_string16.h>
#include <bal_pointer_list.h>
#include <bal_defs.h>
#include <windows.h>

void LOCAL_FUNCTION LF_BalEntry(PPointerList args);
void LOCAL_FUNCTION LF_BalExit(PPointerList args);
void BalMemoryInit();
Int main(const PPointerList args);

INT WINAPI PenguEntry(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR     lpCmdLine,
    INT       nShowCmd
) {
    BString16 _argString = BString16CreateLocal((PChar16)lpCmdLine);
    PPointerList _argList = BString16Split(&_argString, ' ');
    LF_BalEntry(_argList);
    Int _res = main(_argList);
    LF_BalExit(_argList);
    if (_argList)
        PListDestroy(_argList);
    return (INT)_res;
}

void LOCAL_FUNCTION LF_BalEntry(PPointerList args)
{
    BalMemoryInit();
}

void LOCAL_FUNCTION LF_BalExit(PPointerList args)
{
}