#ifndef SYNC_QUEUE_H
#define SYNC_QUEUE_H

#include <semaphore.h>

typedef struct {
    sem_t _r_lock;
    sem_t _num_elems_lock;
    sem_t _w_lock;
    sem_t _free_elems_lock;
    int _max_size;
    int _push_index;
    int _pop_index;
    void **_data;
} sync_queue;

sync_queue* sync_queue_new(int max_size);
void sync_queue_free(sync_queue *queue);

void sync_queue_push(sync_queue *queue, void *data);
void *sync_queue_pop(sync_queue *queue);

#endif