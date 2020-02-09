#ifdef GREEN_WORKERS
    #include "green_threads.h"
    #define thread_impl_t gthread_t
    #define thread_impl_create gthread_create
    #define thread_impl_join gthread_join
#else
    #include <pthread.h>
    #define thread_impl_t pthread_t
    #define thread_impl_create pthread_create
    #define thread_impl_join pthread_join
#endif

#include <stdio.h>
#include <stdlib.h>

#include "promise.h"

typedef struct array_summing {
    int num_elems;
    int *elems;
    promise *output;
} array_summing;

void *worker(void *params){
    array_summing *array_sum_params = params;

    int *sum = malloc(sizeof(int));
    *sum = 0;

    for(int index = 0; index < array_sum_params->num_elems; index ++){
        *sum += array_sum_params->elems[index];
    }

    promise_fulfill(array_sum_params->output, sum);
    return NULL;
}

int main(void){
    int n_threads = 29 * 1024;
    int num_elems = 1;

    array_summing *array_sum_params_list = malloc(n_threads * sizeof(array_summing));
    thread_impl_t *threads = malloc(n_threads * sizeof(thread_impl_t));

    int total_num_elems = n_threads * num_elems;
    int *big_array = malloc(total_num_elems * sizeof(int));

    for(int index = 0; index < total_num_elems; index ++){
        big_array[index] = index;
    }

    for(int index = 0; index < n_threads; index ++){
        array_sum_params_list[index].num_elems = num_elems;
        array_sum_params_list[index].elems = big_array + index * num_elems;
        array_sum_params_list[index].output = promise_new();

        thread_impl_create(&(threads[index]), NULL, worker, &(array_sum_params_list[index]));
    }

    int total = 0;

    for(int index = 0; index < n_threads; index ++){
        total += *(int *)promise_await(array_sum_params_list[index].output);
        thread_impl_join(threads[index], NULL);
        promise_free(array_sum_params_list[index].output);
    }

    free(array_sum_params_list);
    free(big_array);

    // sum from 0 to total_num_elems - 1
    int expected_total = (total_num_elems * (total_num_elems - 1)) / 2;

    printf("%d (should be %d)\n", total, expected_total);

    return 0;
}