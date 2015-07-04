
#if defined(_MSC_VER) && !defined(_WIN64)

#include <windows.h>
#if (_WIN32_WINNT < 0x0502)
void	MemoryBarrier() {
    LONG Barrier;
    __asm {
        xchg Barrier, eax
    }
}
#endif

#endif
