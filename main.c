#include <stdlib.h>
#ifdef _WIN32
# include <crtdbg.h>
#endif




#include "i.h"



void taskFn(void* ctx)
{

}




static void test(void)
{
    TE_init();

    //TE_Task task = { taskFn,  };
    //TE_exe(&task);

    TE_deinit();
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



















































































