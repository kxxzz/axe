#pragma once



typedef struct TE_ThrdPool TE_ThrdPool;

TE_ThrdPool* TE_new_thrdPool(u32 threadCount);
void TE_thrdPool_free(TE_ThrdPool* pool);
void TE_thrdPool_add(TE_ThrdPool* pool, TE_TaskFn fn, TE_Ctx* ctx);
































































































