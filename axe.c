#include "axe_a.h"




static AXE_ThrdPool* s_thrdPool;



static u32 getNumCores(void)
{
#if defined(_WIN32)
    SYSTEM_INFO sysinfo[1];
    GetSystemInfo(sysinfo);
    return sysinfo->dwNumberOfProcessors;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}


void AXE_init(u32 n)
{
    assert(!s_thrdPool);
    if (!n)
    {
        u32 n = getNumCores();
        s_thrdPool = AXE_thrdPoolNew(n * 2);
    }
    else
    {
        s_thrdPool = AXE_thrdPoolNew(n);
    }
    assert(s_thrdPool);
}


void AXE_deinit(void)
{
    assert(s_thrdPool);
    AXE_thrdPoolFree(s_thrdPool);
    s_thrdPool = NULL;
}


bool AXE_exe(AXE_TaskFn fn, void* ctx, int64_t* done)
{
    assert(s_thrdPool);
    return AXE_thrdPoolAdd(s_thrdPool, fn, ctx, done);
}

















































































