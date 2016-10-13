#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

/**
 *@defgroup Tasks
 *@{
 */

/**
 * @struct _task threadpool.h "threadpool.h"
 * @brief The data structure for a task which will be put in a task queue
 */
typedef struct _task {
    void (*func)(void *);	///<pointer to task function
    void *arg;			///<pointer to the arguments passed to _func_
    struct _task *next;		///<pointer to the next task
    struct _task *last;		///<poniter to the previous task
} task_t;

int task_free(task_t *the_task);

/** @} */

/**
 * @defgroup Task Queue
 * @{
 */
/**
 * @struct _tqueue_t threadpool.h "threadpool.h"
 * @brief The data structure for a task queue of _task\_t_
 */
typedef struct _tqueue_t {
    task_t *head;		///<pointer to the first _task\_t_in in queue
    task_t *tail;		///<pointer to the last _task\_t_in queue
    pthread_mutex_t mutex;	///<The mutex lock of this queue
    pthread_cond_t cond;	///<The conditional variable of this queue
    uint32_t size;		///<The size of queue
} tqueue_t;

int tqueue_init(tqueue_t *the_queue);
task_t *tqueue_pop(tqueue_t *the_queue);
uint32_t tqueue_size(tqueue_t *the_queue);
int tqueue_push(tqueue_t *the_queue, task_t *task);
int tqueue_free(tqueue_t *the_queue);

/** @} */
/**
 *defgroup Thread pool
 * @{
 */

/**
 * @struct _tpool_t threadpool.h "threadpool.h"
 * @brief The data structure for managing thread pool
 */

typedef struct tpool_t {
    pthread_t *threads;		///<pointer to the array of threads
    uint32_t count;		///<Number of working threads
    tqueue_t *queue;		///<pointer to the task queue
} tpool_t;

int tpool_init(tpool_t *the_pool, uint32_t count, void *(*func)(void *));
int tpool_free(tpool_t *the_pool);
/** @} */
#endif
