# green-threads

## what is this

Simple green threads implementation in C.  I wanted to create green threads versions of `pthread_create` and `pthread_join`.  Check out [green_threads.h](./inc/green_threads.h) for interface definitions and [green_threads.c](./src/green_threads.c) for implementation.

## long story short

This implementation is basically a number of pthreads (= number of hardware threads) pulling from a work-stealing queue where the queue contains `workloads`, which themselves are a pair of callback-to-run and its arguments.  My implementation implies that the green threads are cooperative - if the worker pthreads get occupied with green thread tasks that never end, they will not be able to execute any others.

## how to build

`make compare` to run the simple benchmark defined in [main.c](./src/main.c).

### dependencies

gcc, pthreads lib

## etc

Also included is a toy implementation for promises.  Just used for the benchmark.

## todo
* set cpu affinity for worker pthreads