#include <bal_defs.h>
#include "bal_memory.h"
#ifdef BAL_MEM_KERNEL
#include <ntifs.h>
#else
#include <windows.h>
#endif

LOCAL_VARIABLE HANDLE LV_processHeap;

void BalMemoryInit()
{
#ifdef BAL_MEM_KERNEL
    LV_processHeap = RtlCreateHeap(HEAP_GROWABLE, 0, 0, 0, 0, 0);
#else
    LV_processHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
#endif
}

pvoid BalAlloc(ULong size)
{
#ifdef BAL_MEM_KERNEL
    return RtlAllocateHeap(LV_processHeap, 0, size);
#else
    return HeapAlloc(LV_processHeap, 0, size);
#endif
}

pvoid BalAllocZero(ULong size)
{
#ifdef BAL_MEM_KERNEL
    return RtlAllocateHeap(LV_processHeap, HEAP_ZERO_MEMORY, size);
#else
    return HeapAlloc(LV_processHeap, HEAP_ZERO_MEMORY, size);
#endif
}

pvoid BalAllocExecutable(ULong size)
{
#ifdef BAL_MEM_KERNEL
    pvoid _ptr = 0;
    if (ZwAllocateVirtualMemory(LV_processHeap, &_ptr, 0, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE))
    {
        return _ptr;
    }
    return 0;
#else
    return HeapAlloc(LV_processHeap, 0, size);
#endif
}

pvoid BalReAlloc(pvoid ptr, ULong newSize)
{
#ifdef BAL_MEM_KERNEL
    return 0;
#else
    return HeapReAlloc(LV_processHeap, 0, ptr, newSize);
#endif
}

Bool BalAllocVirtual(pvoid* address, Bool executable, ULong size)
{
#ifdef BAL_MEM_KERNEL
    return ZwAllocateVirtualMemory(LV_processHeap, address, 0, &size, MEM_COMMIT, executable ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE);
#else
    return (Bool)VirtualAlloc((LPVOID)address, size, 0, executable ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE);
#endif
}

void BalFree(pvoid ptr)
{
#ifdef BAL_MEM_KERNEL
    RtlFreeHeap(LV_processHeap, 0, ptr);
#else
    HeapFree(LV_processHeap, 0, ptr);
#endif
}