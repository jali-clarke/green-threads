#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#include "green_threads.h"
#include "sync_queue.h"

typedef struct rts_state {
    pthread_t *_workers;
    sync_queue *_work_queue;
} rts_state;

typedef struct workload {
    sem_t *_semaphore;
    void *(* _callback)(void *);
    void *_data;
} workload;

rts_state *_rts_state = NULL;

void *_worker_thread_routine(void *ignored){
    while(1){
        workload *to_run = sync_queue_pop(_rts_state->_work_queue);

        void *(* callback)(void *) = to_run->_callback;
        void *data = to_run->_data;

        callback(data);

        sem_post(to_run->_semaphore);
        free(to_run);
    }

    return NULL;
}

void _gthread_init(int num_workers, int queue_size){
    if(_rts_state == NULL){
        _rts_state = malloc(sizeof(rts_state));
        _rts_state->_workers = malloc(num_workers * sizeof(pthread_t));
        _rts_state->_work_queue = sync_queue_new(queue_size);

        for(int worker_index = 0; worker_index < num_workers; worker_index++){
            pthread_create(_rts_state->_workers + worker_index, NULL, _worker_thread_routine, NULL);
            pthread_detach(_rts_state->_workers[worker_index]);
        }
    }
}

int gthread_create(gthread_t *thread, const gthread_attr_t *attr, void *(* start_routine)(void *), void *arg){
    // below macros from D flag - see Makefile
    _gthread_init(GREEN_WORKERS, GREEN_QUEUE_SIZE);

    sem_t *semaphore = malloc(sizeof(sem_t));
    sem_init(semaphore, 0, 0);

    thread->_semaphore = semaphore;

    workload *to_execute = malloc(sizeof(workload));
    to_execute->_semaphore = semaphore;
    to_execute->_callback = start_routine;
    to_execute->_data = arg;

    sync_queue_push(_rts_state->_work_queue, to_execute);

    return 0;
}

int gthread_join(gthread_t thread, void **retval){
    return sem_wait(thread._semaphore);
}