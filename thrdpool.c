#include "a.h"



enum
{
    TE_ThrdPool_QueueSize = 1024,
};



typedef struct TE_ThrdTask
{
    TE_ThrdFn fn;
    void* arg;
} TE_ThrdTask;



typedef struct TE_ThrdPool
{
    mtx_t lock;
    cnd_t notify;
    thrd_t* threads;
    u32 threadCount;
    TE_ThrdTask* queue;
    s32 head;
    s32 tail;
    bool shutdown;
} TE_ThrdPool;




static s32 TE_thrdPool_worker(TE_ThrdPool* pool)
{
    TE_ThrdTask task;
    for (;;)
    {
        mtx_lock(&pool->lock);
        {
            while ((pool->tail == pool->head) && !pool->shutdown)
            {
                cnd_wait(&pool->notify, &pool->lock);
            }
            if (pool->shutdown)
            {
                mtx_unlock(&pool->lock);
                break;
            }

            task.fn = pool->queue[pool->head].fn;
            task.arg = pool->queue[pool->head].arg;
            pool->head = (pool->head + 1) % TE_ThrdPool_QueueSize;
        }
        mtx_unlock(&pool->lock);

        task.fn(task.arg);
    }
    return thrd_success;
}




TE_ThrdPool* TE_new_thrdPool(u32 threadCount)
{
    TE_ThrdPool* pool = zalloc(sizeof(TE_ThrdPool));

    pool->threadCount = threadCount;
    pool->threads = zalloc(sizeof(thrd_t)*pool->threadCount);
    pool->queue = zalloc(sizeof(TE_ThrdTask)*TE_ThrdPool_QueueSize);
    pool->head = 0;
    pool->tail = 0;
    pool->shutdown = false;

    if (mtx_init(&pool->lock, mtx_plain) != thrd_success)
    {
        free(pool->queue);
        free(pool->threads);
        free(pool);
        return NULL;
    }
    if (cnd_init(&pool->notify) != thrd_success)
    {
        free(pool->queue);
        free(pool->threads);
        mtx_lock(&pool->lock);
        mtx_destroy(&pool->lock);
        free(pool);
        return NULL;
    }

    for (u32 i = 0; i < pool->threadCount; ++i)
    {
        if (thrd_create(&pool->threads[i], (thrd_start_t)TE_thrdPool_worker, pool) != thrd_success)
        {
            free(pool->queue);
            free(pool->threads);
            mtx_lock(&pool->lock);
            mtx_destroy(&pool->lock);
            cnd_destroy(&pool->notify);
            free(pool);
            return NULL;
        }
    }
    return pool;
}





void TE_thrdPool_free(TE_ThrdPool* pool)
{
    mtx_lock(&pool->lock);
    {
        pool->shutdown = true;
        cnd_broadcast(&pool->notify);
    }
    mtx_unlock(&pool->lock);
    for (u32 i = 0; i < pool->threadCount; ++i)
    {
        s32 r = thrd_join(pool->threads[i], NULL);
        assert(thrd_success == r);
    }

    free(pool->queue);
    free(pool->threads);
    mtx_lock(&pool->lock);
    mtx_destroy(&pool->lock);
    cnd_destroy(&pool->notify);
    free(pool);
}





void TE_thrdPool_add(TE_ThrdPool* pool, TE_ThrdFn fn, void* arg)
{
    mtx_lock(&pool->lock);

    while ((pool->tail - pool->head == TE_ThrdPool_QueueSize) && !pool->shutdown)
    {
        cnd_wait(&pool->notify, &pool->lock);
    }
    if (pool->shutdown)
    {
        mtx_unlock(&pool->lock);
        return;
    }

    pool->queue[pool->tail].fn = fn;
    pool->queue[pool->tail].arg = arg;
    pool->tail = (pool->tail + 1) % TE_ThrdPool_QueueSize;

    cnd_signal(&pool->notify);

    mtx_unlock(&pool->lock);
}




























































































