#include "a.h"





static TE_ThrdPool* s_thrdPool;



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


void TE_init(void)
{
    assert(!s_thrdPool);
    u32 n = getNumCores();
    s_thrdPool = TE_new_thrdPool(n * 2);
}


void TE_deinit(void)
{
    assert(s_thrdPool);
    TE_thrdPool_free(s_thrdPool);
    s_thrdPool = NULL;
}


void TE_exe(TE_Task* task)
{
    TE_thrdPool_add(s_thrdPool, task->fn, task->ctx);
}

















































































