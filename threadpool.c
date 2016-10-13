#include "threadpool.h"

/**
 * @brief Release the memory allocated in _task\_t_
 * @param the_task Pointer to the target _task\_t_
*/
int task_free(task_t *the_task)
{
    free(the_task->arg);
    free(the_task);
    // free(the_task);
    return 0;
}

/**
 * @brief Initialize the members in _tqueue\_t_.
 *
 * Note that this function doesn't allocate memory for the task queue.
 *
 * @param the_queue Pointer to the target _tqueue\_t_
 * @return -1 if _the\_queue_ is NULL. Otherwise, return 0.
 */

int tqueue_init(tqueue_t *the_queue)
{
    //TODO:Check if the input pointer is vaild
    if(the_queue==NULL)
        return -1;
    the_queue->head = NULL;
    the_queue->tail = NULL;
    pthread_mutex_init(&(the_queue->mutex), NULL);
    pthread_cond_init(&(the_queue->cond), NULL);
    the_queue->size = 0;
    return 0;
}

/**
 * @brief Pop a task from the task queue.
 * @param the_queue Pointer to the target task queue
 * @return The popped task. NULL if the queue is empty.
 */
task_t *tqueue_pop(tqueue_t *the_queue)
{
    task_t *ret;
    pthread_mutex_lock(&(the_queue->mutex));
    //FIXME Pop the task from the head of the queue.
    ret = the_queue->head;
    if (ret) {
        the_queue->head = ret->next;
        if (the_queue->head) {
            the_queue->head->last = NULL;
        } else {
            the_queue->tail = NULL;
        }
        the_queue->size--;
    }
    pthread_mutex_unlock(&(the_queue->mutex));
    return ret;
}

/**
 * @brief Get the number of the tasks in the task queue
 * @param the_queue Pointer to the target task queue
 * @return The number of the tasks in the task queue
 */

uint32_t tqueue_size(tqueue_t *the_queue)
{
    uint32_t ret;
    pthread_mutex_lock(&(the_queue->mutex));
    ret = the_queue->size;
    pthread_mutex_unlock(&(the_queue->mutex));
    return ret;
}

/**
 * @brief Push a task to the task queue
 * @param the_queue Pointer to the target task queue
 * @param task Pointer to the task to push
 */

int tqueue_push(tqueue_t *the_queue, task_t *task)
{
    pthread_mutex_lock(&(the_queue->mutex));
    // FIXME: Push the new task to the end of the queue.
    task->last = the_queue->tail;
    task->next = NULL;
    if (the_queue->tail)
        the_queue->tail->next = task;
    the_queue->tail = task;
    if (the_queue->size++ == 0)
        the_queue->head = task;
    pthread_mutex_unlock(&(the_queue->mutex));
    return 0;
}

/**
 * @brief Release the memory of the remaining tasks in the task queue and destory the mutex lock
 * @param the_queue The target task queue
 */

int tqueue_free(tqueue_t *the_queue)
{
    task_t *cur = the_queue->head;
    while (cur) {
        the_queue->head = the_queue->head->next;
        // FIXME: Use task_free() instead.
        free(cur);
        cur = the_queue->head;
    }
    pthread_mutex_destroy(&(the_queue->mutex));
    return 0;
}

/**
 * @brief Create and initialize the thread pool
 * @param the_pool Pointer to the created thread pool
 * @param tcount The number of the threads to be created
 * @param func Pointer to the task function
 */

int tpool_init(tpool_t *the_pool, uint32_t tcount, void *(*func)(void *))
{
    the_pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * tcount);
    the_pool->count = tcount;
    the_pool->queue = (tqueue_t *) malloc(sizeof(tqueue_t));
    tqueue_init(the_pool->queue);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (uint32_t i = 0; i < tcount; ++i)
        pthread_create(&(the_pool->threads[i]), &attr, func, NULL);
    pthread_attr_destroy(&attr);
    return 0;
}

/**
 * @brief Join all the threads and release the allocated memory of thread pool
 * @param the_pool The target thread pool
 */

int tpool_free(tpool_t *the_pool)
{
    for (uint32_t i = 0; i < the_pool->count; ++i)
        pthread_join(the_pool->threads[i], NULL);
    free(the_pool->threads);
    tqueue_free(the_pool->queue);
    return 0;
}
