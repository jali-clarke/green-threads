#include <semaphore.h>
#include <stdlib.h>

#include "sync_queue.h"

struct sync_queue {
    sem_t _r_lock;
    sem_t _num_elems_lock;
    sem_t _w_lock;
    sem_t _free_elems_lock;
    int _max_size;
    int _push_index;
    int _pop_index;
    void **_data;
}; // typedef in header

sync_queue* sync_queue_new(int max_size){
    sync_queue* queue = malloc(sizeof(sync_queue));
    
    sem_init(&(queue->_r_lock), 0, 1);
    sem_init(&(queue->_num_elems_lock), 0, 0);
    sem_init(&(queue->_w_lock), 0, 1);
    sem_init(&(queue->_free_elems_lock), 0, max_size);

    queue->_max_size = max_size;
    queue->_push_index = 0;
    queue->_pop_index = 0;
    queue->_data = malloc(max_size * sizeof(void *));

    return queue;
}

void sync_queue_free(sync_queue *queue){
    free(queue->_data);
    free(queue);
}

void sync_queue_push(sync_queue *queue, void *data){
    sem_wait(&(queue->_w_lock));
    sem_wait(&(queue->_free_elems_lock));

    queue->_data[queue->_push_index] = data;
    queue->_push_index = (queue->_push_index + 1) % queue->_max_size;

    sem_post(&(queue->_num_elems_lock));
    sem_post(&(queue->_w_lock));
}

void *sync_queue_pop(sync_queue *queue){
    sem_wait(&(queue->_r_lock));
    sem_wait(&(queue->_num_elems_lock));

    void* data = queue->_data[queue->_pop_index];
    queue->_pop_index = (queue->_pop_index + 1) % queue->_max_size;

    sem_post(&(queue->_free_elems_lock));
    sem_post(&(queue->_r_lock));

    return data;
}