#pragma once



#include "texe.h"



#ifdef ARYLEN
# undef ARYLEN
#endif
#define ARYLEN(a) (sizeof(a) / sizeof((a)[0]))



#ifdef _MSC_VER
# pragma warning (disable : 4305)
#endif

#ifdef __EMSCRIPTEN__
# define HAVE_PTHREAD
# define HAVE_TIMESPEC_GET
#endif


#include <time.h>
#include <string.h>
#include <stdio.h>
#include <atomic.h>
#include <sleep.h>
#include <threads.h>

#include "texe_thrdpool.h"



#define zalloc(sz) calloc(1, sz)






















































