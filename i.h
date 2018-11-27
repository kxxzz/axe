#pragma once




#include <stdbool.h>
#include <stdint.h>



typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;





typedef void(*TE_TaskFn)(struct TE_Task* task);

typedef struct TE_Task
{
    TE_TaskFn fn;
    void* ctx;
    int64_t done;
} TE_Task;

void TE_exe(TE_Task* task);




































































