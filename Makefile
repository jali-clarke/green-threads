.PHONY: clean, os_threads, green_threads, compare

GREEN_NUM_CPUS ?= $(shell lscpu | sed -nr 's/^CPU\(s\):[^0-9]*([0-9]+)$$/\1/p')
GREEN_QUEUE_SIZE ?= 128

GREEN_FLAGS = -DGREEN_NUM_CPUS=$(GREEN_NUM_CPUS) -DGREEN_QUEUE_SIZE=$(GREEN_QUEUE_SIZE)

GCC = gcc -Iinc

BUILD_MAIN = mkdir -p bin && $(GCC) -pthread -o $@ $^
BUILD_LIB = mkdir -p obj && $(GCC) -c -o $@ $<
BUILD_GREEN_LIB = $(GCC) $(GREEN_FLAGS) -c -o $@ $<

compare: os_threads green_threads
	bash -c "time ./bin/os_threads"
	bash -c "time ./bin/green_threads"

os_threads: bin/os_threads

green_threads: bin/green_threads

bin/os_threads: obj/os_threads_main.o obj/promise.o
	$(BUILD_MAIN)

bin/green_threads: obj/green_threads_main.o obj/green_threads.o obj/sync_queue.o obj/promise.o
	$(BUILD_MAIN)

obj/os_threads_main.o: src/main.c inc/promise.h
	$(BUILD_LIB)

obj/green_threads_main.o: src/main.c inc/green_threads.h inc/promise.h
	$(BUILD_GREEN_LIB)

obj/green_threads.o: src/green_threads.c inc/sync_queue.h
	$(BUILD_GREEN_LIB)

obj/sync_queue.o: src/sync_queue.c inc/sync_queue.h
	$(BUILD_LIB)

obj/promise.o: src/promise.c inc/promise.h
	$(BUILD_LIB)

clean:
	rm -rf obj bin