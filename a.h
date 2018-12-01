#pragma once



#include "i.h"



#ifdef ARYLEN
# undef ARYLEN
#endif
#define ARYLEN(a) (sizeof(a) / sizeof((a)[0]))



#ifdef _MSC_VER
# pragma warning (disable : 4305)
#endif



#include <atomic.h>
#include <sleep.h>



#ifdef __EMSCRIPTEN__
# define HAVE_PTHREAD
# define HAVE_TIMESPEC_GET
#endif
#include <threads.h>


#include "thrdpool.h"




void* zalloc(size_t size);






















































