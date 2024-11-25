#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

const int NUM_THREADS = 10;
const int INCREMENTS_PER_THREAD = 100000;

int shared_counter = 0; 
pthread_mutex_t counter_mutex;

void* increment_counter(void* threadid) {
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        pthread_mutex_lock(&counter_mutex);
        shared_counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&counter_mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_counter, (void*)(intptr_t)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("value: %d\n", shared_counter);

    pthread_mutex_destroy(&counter_mutex);

    return 0;
}