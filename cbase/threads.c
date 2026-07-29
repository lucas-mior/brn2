// SPDX-License-Identifier: AGPL
// Copyright (c) 2026 Lucas Mior

#if !defined(THREADS_C)
#define THREADS_C

#include "cbase.h"

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_threads 1
#elif !defined(TESTING_threads)
#define TESTING_threads 0
#endif

#if !defined(PARALLEL_FOR_MAX_THREADS)
#define PARALLEL_FOR_MAX_THREADS 64
#endif

#if !defined(MIN_PARALLEL_ITEMS)
#define MIN_PARALLEL_ITEMS 64
#endif

typedef struct ThreadWork {
    ParallelForFunction *function;
    void *user_data;
    int64 start;
    int64 end;
    int32 worker_id;
    int32 padding;
} ThreadWork;

#if OS_UNIX && (PARALLEL_FOR_MAX_THREADS > 1)
static pthread_mutex_t thread_pool_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t thread_pool_new_work = PTHREAD_COND_INITIALIZER;
static pthread_cond_t thread_pool_done_work = PTHREAD_COND_INITIALIZER;
static pthread_t thread_pool_threads[PARALLEL_FOR_MAX_THREADS];
static int32 thread_pool_nthreads = 0;
static bool thread_pool_is_stopping = false;
static int32 thread_pool_work_pending = 0;
static bool thread_pool_is_initialized = false;

static struct ThreadWorkQueue {
    ThreadWork *items[PARALLEL_FOR_MAX_THREADS];
    int32 head;
    int32 tail;
    int32 count;
    int32 padding;
} thread_pool_queue = {0};

static void
thread_pool_shutdown(void) {
    xpthread_mutex_lock(&thread_pool_mutex);
    thread_pool_is_stopping = true;
    pthread_cond_broadcast(&thread_pool_new_work);
    xpthread_mutex_unlock(&thread_pool_mutex);

    for (int32 i = 0; i < thread_pool_nthreads; i += 1) {
        xpthread_join(&thread_pool_threads[i], NULL);
    }

    thread_pool_nthreads = 0;
    return;
}

static void *
thread_pool_worker(void *arg) {
    (void)arg;

    while (true) {
        ThreadWork *work = NULL;

        xpthread_mutex_lock(&thread_pool_mutex);
        while ((thread_pool_queue.count <= 0)
               && !thread_pool_is_stopping) {
            pthread_cond_wait(&thread_pool_new_work, &thread_pool_mutex);
        }

        if (thread_pool_is_stopping) {
            xpthread_mutex_unlock(&thread_pool_mutex);
            return NULL;
        }

        if (thread_pool_queue.count > 0) {
            work = thread_pool_queue.items[thread_pool_queue.head];
            thread_pool_queue.head = (thread_pool_queue.head + 1)
                                     % LENGTH(thread_pool_queue.items);
            thread_pool_queue.count -= 1;
        }
        xpthread_mutex_unlock(&thread_pool_mutex);

        if (work) {
            work->function(work->start, work->end, work->worker_id,
                           work->user_data);

            xpthread_mutex_lock(&thread_pool_mutex);
            thread_pool_work_pending -= 1;
            if ((thread_pool_work_pending <= 0)
                && (thread_pool_queue.count <= 0)) {
                pthread_cond_signal(&thread_pool_done_work);
            }
            xpthread_mutex_unlock(&thread_pool_mutex);
        }
    }
}

static int32
thread_pool_default_thread_count(
    int64 length,
    int32 max_threads,
    int64 min_parallel_items
) {
    int32 available_threads;
    int32 thread_count;

    if (length <= 0) {
        return 0;
    }
    if (max_threads <= 1) {
        return 1;
    }
    if (length < min_parallel_items) {
        return 1;
    }

    available_threads = util_nthreads();
    if (available_threads <= 0) {
        available_threads = 1;
    }

    thread_count = (int32)MIN((int64)available_threads,
                             (int64)PARALLEL_FOR_MAX_THREADS);
    thread_count = (int32)MIN((int64)thread_count, (int64)max_threads);
    thread_count = (int32)MIN((int64)thread_count, length);
    if (thread_count < 1) {
        thread_count = 1;
    }
    return thread_count;
}

static void
thread_pool_ensure_started(int32 thread_count) {
    if (thread_count <= thread_pool_nthreads) {
        return;
    }

    if (!thread_pool_is_initialized) {
        thread_pool_is_initialized = true;
        atexit(thread_pool_shutdown);
    }

    for (int32 i = thread_pool_nthreads; i < thread_count; i += 1) {
        xpthread_create(&thread_pool_threads[i], NULL, thread_pool_worker,
                        NULL);
    }
    thread_pool_nthreads = thread_count;
    return;
}

CBASE_API_DEF int32
parallel_for_max_threads_min_items(
    int64 length,
    int32 max_threads,
    int64 min_parallel_items,
    ParallelForFunction *function,
    void *user_data
) {
    static ThreadWork slices[PARALLEL_FOR_MAX_THREADS];
    int32 thread_count;
    int64 range;

    if (length <= 0) {
        return 0;
    }

    thread_count = thread_pool_default_thread_count(length, max_threads,
                                                   min_parallel_items);
    if (thread_count <= 1) {
        function(0, length, 0, user_data);
        return 1;
    }

    thread_pool_ensure_started(thread_count);
    range = length / thread_count;

    xpthread_mutex_lock(&thread_pool_mutex);
    for (int32 i = 0; i < thread_count; i += 1) {
        slices[i].function = function;
        slices[i].user_data = user_data;
        slices[i].start = i*range;
        if ((i + 1) < thread_count) {
            slices[i].end = (i + 1)*range;
        } else {
            slices[i].end = length;
        }
        slices[i].worker_id = i;

        if (thread_pool_queue.count >= LENGTH(thread_pool_queue.items)) {
            xpthread_mutex_unlock(&thread_pool_mutex);
            error("Error: Thread pool work queue is full.\n");
            fatal(EXIT_FAILURE);
        }

        thread_pool_queue.items[thread_pool_queue.tail] = &slices[i];
        thread_pool_queue.tail = (thread_pool_queue.tail + 1)
                                    % LENGTH(thread_pool_queue.items);
        thread_pool_queue.count += 1;
        thread_pool_work_pending += 1;
        pthread_cond_signal(&thread_pool_new_work);
    }

    while ((thread_pool_work_pending > 0)
           || (thread_pool_queue.count > 0)) {
        pthread_cond_wait(&thread_pool_done_work, &thread_pool_mutex);
    }
    xpthread_mutex_unlock(&thread_pool_mutex);

    return thread_count;
}
#else
CBASE_API_DEF int32
parallel_for_max_threads_min_items(
    int64 length,
    int32 max_threads,
    int64 min_parallel_items,
    ParallelForFunction *function,
    void *user_data
) {
    (void)max_threads;
    (void)min_parallel_items;

    if (length <= 0) {
        return 0;
    }

    function(0, length, 0, user_data);
    return 1;
}
#endif

CBASE_API_DEF int32
parallel_for_min_items(int64 length, int64 min_parallel_items,
                       ParallelForFunction *function, void *user_data) {
    return parallel_for_max_threads_min_items(length, PARALLEL_FOR_MAX_THREADS,
                                             min_parallel_items, function,
                                             user_data);
}

CBASE_API_DEF int32
parallel_for(int64 length, ParallelForFunction *function, void *user_data) {
    return parallel_for_min_items(length, MIN_PARALLEL_ITEMS, function,
                                  user_data);
}

#if 0 == TESTING_threads
CBASE_API_DEF void
threads_functions_sink(void) {
    (void)parallel_for;
    (void)parallel_for_min_items;
    (void)parallel_for_max_threads_min_items;
    return;
}
#endif

#if TESTING_threads
#define CBASE_IMPLEMENT
#include "cbase.h"

typedef struct ThreadsTestWork {
    int64 *values;
} ThreadsTestWork;

static void
threads_test_fill_work(int64 start, int64 end, int32 worker_id,
                       void *user_data) {
    ThreadsTestWork *work = user_data;

    for (int64 i = start; i < end; i += 1) {
        work->values[i] = i + worker_id;
    }
    return;
}

int32
main(void) {
    int64 length = 10100100;
    int64 *values = malloc2(length*SIZEOF(*values));
    ThreadsTestWork work;
    int32 workers;

    memset64(values, 0, length*SIZEOF(*values));
    work.values = values;
    workers = parallel_for_min_items(length, 1, threads_test_fill_work,
                                     &work);

    ASSERT_MORE_EQUAL(workers, 1);
    for (int64 i = 0; i < length; i += 1) {
        ASSERT_MORE_EQUAL(values[i], i);
        ASSERT_LESS(values[i], i + PARALLEL_FOR_MAX_THREADS);
    }

    free2(values, length*SIZEOF(*values));
    exit(EXIT_SUCCESS);
}
#endif /* TESTING_threads */

#endif /* THREADS_C */
