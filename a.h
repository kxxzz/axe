#pragma once



#include "i.h"



#ifdef _MSC_VER
# pragma warning (disable : 4305)
#endif



#include <atomic.h>



#ifdef __EMSCRIPTEN__
# define HAVE_PTHREAD
# define HAVE_TIMESPEC_GET
#endif
#include <threads.h>


#include "thrdpool.h"




void* zalloc(size_t size);






















































