#include "a.h"




static TEXE_ThrdPool* s_thrdPool;



static u32 getNumCores(void)
{
#if defined(_WIN32)
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}


void TEXE_init(u32 n)
{
    assert(!s_thrdPool);
    if (!n)
    {
        u32 n = getNumCores();
        s_thrdPool = TEXE_new_thrdPool(n * 2);
    }
    else
    {
        s_thrdPool = TEXE_new_thrdPool(n);
    }
    assert(s_thrdPool);
}


void TEXE_deinit(void)
{
    assert(s_thrdPool);
    TEXE_thrdPool_free(s_thrdPool);
    s_thrdPool = NULL;
}


bool TEXE_exe(TEXE_TaskFn fn, void* ctx, int64_t* done)
{
    assert(s_thrdPool);
    return TEXE_thrdPool_add(s_thrdPool, fn, ctx, done);
}

















































































