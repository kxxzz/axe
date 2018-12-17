#include "a.h"




static texe_ThrdPool* s_thrdPool;



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


void texe_init(void)
{
    assert(!s_thrdPool);
    u32 n = getNumCores();
    s_thrdPool = texe_new_thrdPool(n * 2);
    assert(s_thrdPool);
}


void texe_deinit(void)
{
    assert(s_thrdPool);
    texe_thrdPool_free(s_thrdPool);
    s_thrdPool = NULL;
}


bool texe_exe(texe_TaskFn fn, void* ctx, int64_t* done)
{
    assert(s_thrdPool);
    return texe_thrdPool_add(s_thrdPool, fn, ctx, done);
}

















































































