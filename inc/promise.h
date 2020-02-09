#ifndef PROMISE_H
#define PROMISE_H

#include <semaphore.h>

typedef struct {
    sem_t _lock;
    void *_data;
} promise;

promise *promise_new(void);
void promise_fulfill(promise *promise_to_fulfill, void *data);
void *promise_await(promise *fulfilled_promise);
void promise_free(promise *promise_to_free);

#endif