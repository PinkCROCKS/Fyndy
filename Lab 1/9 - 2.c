#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable : 4996)

typedef enum Errors {
    zero,
    amount_of_arguments_error,
    input_error, 
    memory_error
} Errors;

int compare(const void* x1, const void* x2) {   // функция сравнения элементов массива
    return (*(int*)x1 - *(int*)x2);              // если результат вычитания равен 0, то числа равны, < 0: x1 < x2; > 0: x1 > x2
}

int error(int number) {
    switch (number) {
    case amount_of_arguments_error:
        printf("Mistake with arguments");
        break;
    case input_error:
        printf("Incorrect arguments");
        break;
    default:
        return 1;
    }
    return 0;
}

int random(int min, int max, int* array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = rand() % (max - min) + min;
    }
    return 0;
}

int binary_search_nearest(int arr[], int size, int value) {
    int left = 0;
    int right = size - 1;
    int mid;

    while (left <= right) {
        mid = left + (right - left) / 2;

        if (arr[mid] == value) {
            return arr[mid];
        }
        else if (arr[mid] < value) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    if (left == 0) {
        return arr[0];  
    }
    else if (left == size) {
        return arr[size - 1];  
    }
    else {
        if (abs(arr[left] - value) < abs(arr[left - 1] - value)) {
            return arr[left];
        }
        else {
            return arr[left - 1];
        }
    }
}

int main() {
    int size = rand() % (20 - 10) + 10;
    int* A = (int*) malloc(size * sizeof(int));
    if (A == NULL) {
        return 1;
    }
    int* B = (int*) malloc(size * sizeof(int));
    if (B == NULL) {
        free(A);
        return 1;
    }
    random(-10, 10, A, size);
    random(-10, 10, B, size);
    qsort(B, size, sizeof(int), compare);
    int* C = malloc(size * sizeof(int));
    if (C == NULL) {
        free(A);
        free(B);
        return 1;
    }
    for (int i = 0; i < size; i++) {
        C[i] = A[i] + binary_search_nearest(B, size, A[i]);
    }
    printf("A B C\n");
    for (int i = 0; i < size; i++) {
        printf("%d %d %d\n", A[i], B[i], C[i]);
    }
    free(A);
    free(B);
    free(C);
    return 0;
}