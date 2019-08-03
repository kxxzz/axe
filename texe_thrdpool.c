#include "texe_a.h"



enum
{
    TEXE_ThrdPool_QueueSize = 1024,
};



typedef struct TEXE_ThrdTask
{
    TEXE_TaskFn fn;
    void* ctx;
    int64_t* done;
} TEXE_ThrdTask;



typedef struct TEXE_ThrdPool
{
    mtx_t lock[1];
    cnd_t notify[1];
    thrd_t* threads;
    u32 threadCount;
    TEXE_ThrdTask* queue;
    s32 head;
    s32 tail;
    bool shutdown;
} TEXE_ThrdPool;




static s32 TEXE_thrdPoolWorker(TEXE_ThrdPool* pool)
{
    TEXE_ThrdTask task;
    for (;;)
    {
        mtx_lock(pool->lock);
        {
            while ((pool->tail == pool->head) && !pool->shutdown)
            {
                cnd_wait(pool->notify, pool->lock);
            }
            if (pool->shutdown)
            {
                mtx_unlock(pool->lock);
                break;
            }

            task = pool->queue[pool->head];
            pool->head = (pool->head + 1) % TEXE_ThrdPool_QueueSize;
        }
        mtx_unlock(pool->lock);

        task.fn(task.ctx);
        if (task.done)
        {
            atomic_set(task.done, 1);
        }
    }
    return thrd_success;
}




TEXE_ThrdPool* TEXE_thrdPoolNew(u32 threadCount)
{
    TEXE_ThrdPool* pool = zalloc(sizeof(TEXE_ThrdPool));

    pool->threadCount = threadCount;
    pool->threads = zalloc(sizeof(thrd_t)*pool->threadCount);
    pool->queue = zalloc(sizeof(TEXE_ThrdTask)*TEXE_ThrdPool_QueueSize);
    pool->head = 0;
    pool->tail = 0;
    pool->shutdown = false;

    if (mtx_init(pool->lock, mtx_plain) != thrd_success)
    {
        free(pool->queue);
        free(pool->threads);
        free(pool);
        return NULL;
    }
    if (cnd_init(pool->notify) != thrd_success)
    {
        free(pool->queue);
        free(pool->threads);
        mtx_lock(pool->lock);
        mtx_destroy(pool->lock);
        free(pool);
        return NULL;
    }

    for (u32 i = 0; i < pool->threadCount; ++i)
    {
        if (thrd_create(&pool->threads[i], (thrd_start_t)TEXE_thrdPoolWorker, pool) != thrd_success)
        {
            free(pool->queue);
            free(pool->threads);
            mtx_lock(pool->lock);
            mtx_destroy(pool->lock);
            cnd_destroy(pool->notify);
            free(pool);
            return NULL;
        }
    }
    return pool;
}





void TEXE_thrdPoolFree(TEXE_ThrdPool* pool)
{
    mtx_lock(pool->lock);
    {
        pool->shutdown = true;
        cnd_broadcast(pool->notify);
    }
    mtx_unlock(pool->lock);
    for (u32 i = 0; i < pool->threadCount; ++i)
    {
        s32 r = thrd_join(pool->threads[i], NULL);
        assert(thrd_success == r);
    }

    free(pool->queue);
    free(pool->threads);
    mtx_lock(pool->lock);
    mtx_destroy(pool->lock);
    cnd_destroy(pool->notify);
    free(pool);
}





bool TEXE_thrdPoolAdd(TEXE_ThrdPool* pool, TEXE_TaskFn fn, void* ctx, int64_t* done)
{
    mtx_lock(pool->lock);

    while ((pool->tail - pool->head == TEXE_ThrdPool_QueueSize) && !pool->shutdown)
    {
        cnd_wait(pool->notify, pool->lock);
    }
    if (pool->shutdown)
    {
        mtx_unlock(pool->lock);
        return false;
    }
    s32 tail1 = (pool->tail + 1) % TEXE_ThrdPool_QueueSize;
    if (tail1 == pool->head)
    {
        mtx_unlock(pool->lock);
        return false;
    }

    pool->queue[pool->tail].fn = fn;
    pool->queue[pool->tail].ctx = ctx;
    pool->queue[pool->tail].done = done;
    pool->tail = tail1;

    cnd_signal(pool->notify);

    mtx_unlock(pool->lock);
    return true;
}




























































































