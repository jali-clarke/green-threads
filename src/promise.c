#include <semaphore.h>
#include <stdlib.h>

#include "promise.h"

promise *promise_new(void){
    promise *new_promise = malloc(sizeof(promise));

    sem_init(&(new_promise->_lock), 0, 0);
    new_promise->_data = NULL;

    return new_promise;
}

void promise_fulfill(promise *promise_to_fulfill, void *data){
    promise_to_fulfill->_data = data;
    sem_post(&(promise_to_fulfill->_lock));
}

void *promise_await(promise *fulfilled_promise){
    sem_wait(&(fulfilled_promise->_lock));
    return fulfilled_promise->_data;
}

void promise_free(promise *done_promise){
    sem_destroy(&(done_promise->_lock));
    free(done_promise->_data);
    free(done_promise);
}