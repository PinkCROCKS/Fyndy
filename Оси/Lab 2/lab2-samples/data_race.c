#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

const int NUM_THREADS = 10;
const int INCREMENTS_PER_THREAD = 100000;

int shared_counter = 0; 

void* increment_counter(void* threadid) {
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        shared_counter++;
    }
    return NULL;
}
// counter = 5
// thread1 5 + 1 -> counter = 6
// thread2 5 + 1 -> counter = 6

int main() {
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_counter, (void*)(intptr_t)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("value: %d\n", shared_counter);
    return 0;
}