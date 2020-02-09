#ifndef GREEN_THREADS_H
#define GREEN_THREADS_H

#include <semaphore.h>

typedef struct {
    sem_t *_semaphore;
} gthread_t;

typedef void gthread_attr_t;

int gthread_create(gthread_t *thread, const gthread_attr_t *ignored, void *(*start_routine)(void *), void *arg);
int gthread_join(gthread_t thread, void **ignored);

#endif