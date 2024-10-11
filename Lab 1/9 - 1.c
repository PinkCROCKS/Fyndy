#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable : 4996)

typedef enum Errors {
	zero,
    amount_of_arguments_error, 
    input_error
} Errors;

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

int my_atof(const char* string, int* eps) {
    int sign = 1, signs = 0, dots = 0, numbers = 0;
    int result = 0;
    int fraction = 1;
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == '-' && signs == 0) {
            signs = 1;
            sign = -1;
        }
        else if (string[i] == '.' && dots == 0) {
            dots = 1;
            return input_error;
        }
        else if ((string[i] >= '0' && string[i] <= '9') && dots == 0) {
            result *= 10;
            result += (string[i] - '0');
        }
        else {
            return input_error;
        }
    }
    if (result == 0) {
        return input_error;
    }
    *eps = result * sign;
    return 0;
}


int GetOpts(int argc, char** argv,  int* parametrs) {
    if (argc != 3) {
        return amount_of_arguments_error;
    }
    for (int i = 1; i < argc; i++) {
        const char* procceding_option = argv[i];
        int incorrect_input = my_atof(argv[i], &parametrs[i - 1]);
        if (incorrect_input) {
            return incorrect_input;
        }
    }
    return 0;
}

int random(int min, int max, int* array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = rand() % (max - min) + min;
    }
    return 0;
}

int change(int* array, int size) {
    int ind_min = 0;
    int ind_max = 0;
    int min = array[0];
    int max = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] < min) {
            min = array[i];
            ind_min = i;
        }
        else if (array[i] > max) {
            max = array[i];
            ind_max = i;
        }
    }
    if (ind_min != ind_max) {
        int k = array[ind_min];
        array[ind_min] = array[ind_max];
        array[ind_max] = k;
    }
    return 0;
}

int main(int argc, char** argv) {
    int parametrs[2];
    int result = GetOpts(argc, argv, parametrs);
    int t;
    if (parametrs[0] == parametrs[1]) {
        return error(input_error);
    }
    if (parametrs[1] < parametrs[0]) {
        t = parametrs[0];
        parametrs[0] = parametrs[1];
        parametrs[1] = t;
    }
    printf("%d %d\n", parametrs[0], parametrs[1]);
    int array[15];
    if (!(result)) {
        random(parametrs[0], parametrs[1], array, (sizeof(array) / sizeof(int)));
        for (int i = 0; i < 15; i++) {
            array[i] = (rand() % (parametrs[1] - parametrs[0])) + parametrs[0];
            printf("%d ", array[i]);
        }
        change(array, (sizeof(array) / sizeof(int)));
        printf("\n");
        for (int i = 0; i < 15; i++) {
            printf("%d ", array[i]);
        }
    }
    else {
        return error(result);
    }
	return 0;
}