#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int ARRAY_SIZE = 10;
const int NUM_THREADS = 2;

void* multiply_by_two(void* arg) {
    int index = *(int*)arg;
    int offset = NUM_THREADS;

    for (int i = index; i < ARRAY_SIZE; i += offset) {
        array[i] *= 2; 
    }

    return NULL;
}

int main() {
    int array[ARRAY_SIZE];
    pthread_t threads[NUM_THREADS];
    int thread_indices[NUM_THREADS];
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_indices[i] = i; 
        pthread_create(&threads[i], NULL, multiply_by_two, (void*)&thread_indices[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Result: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    return 0;
}