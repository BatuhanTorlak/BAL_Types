#ifndef BAL_MEMORY_H
#define BAL_MEMORY_H
#include "bal_types.h"

pvoid BalAlloc(ULong size);
pvoid BalAllocZero(ULong size);
pvoid BalAllocExecutable(ULong size);
pvoid BalReAlloc(pvoid ptr, ULong newSize);
Bool BalAllocVirtual(pvoid* address, Bool executable, ULong size);
void BalFree(pvoid ptr);

#endif