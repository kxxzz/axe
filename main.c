#include <stdlib.h>
#ifdef _WIN32
# include <crtdbg.h>
#endif



#include <stdio.h>
#include <assert.h>

#include <atomic.h>
#include <sleep.h>

#include "i.h"




static int64_t s_count = 0;


void taskFn(void* ctx)
{
    atomic_inc(&s_count);
    if (1024 == s_count)
    {
        printf("done\n");
    }
}

static void test(void)
{
    TE_init();
    static int64_t done[1024] = { 0 };
    for (u32 i = 0; i < ARYLEN(done); ++i)
    {
        TE_exe(taskFn, NULL, done + i);
    }
    while (!atomic_get(&s_count));
    sleep_ms(1000);
    TE_deinit();
    for (u32 i = 0; i < ARYLEN(done); ++i)
    {
        assert(done[i]);
    }
}





static int mainReturn(int r)
{
#if !defined(NDEBUG) && defined(_WIN32)
    system("pause");
#endif
    return r;
}


int main(int argc, char* argv[])
{
    test();
    mainReturn(0);
}



















































































