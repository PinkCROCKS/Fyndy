//
// Created by Ярослав on 31.10.2024.
//

#ifndef LABA_3_4_ARRAY_H
#define LABA_3_4_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include "strings.h"

typedef struct Array {
    char* val;
    unsigned int length;
    unsigned int capacity;
} Array;

int create_arr(unsigned int length, Array* arr);
int append(Array* arr, char value);
void reset(Array* arr);
int resize(Array* arr, int size_delta);
int extend(Array* arr);
void destroy(Array* arr);
int copy(Array* dst, const Array *src);
int reverse(Array* arr);
void print_arr(Array arr);
void reverse_print_arr(Array arr);
int value_to_arr(unsigned int value, Array* result);
void arr_to_value(Array arr, unsigned int* result);
int value_to_arr_base(unsigned int value, int base, Array* result);
int str_to_arr(char* str, Array* result);
int add(Array A, unsigned int B, Array* result);
int add_arrays(Array A, Array B, Array* result);
int add_arrays_base(Array A, Array B, Array* result, int base);
int multiply(Array A, int B, int base, Array *result);
int multiply_arrays(Array A, Array B, int base, Array* result);
int concat(Array* A, Array B);
int concat_str(Array* A, char* B);
int slice(Array A, int start, int stop, int step, Array* result);
int is_alnum(int x);
int is_num(int x);
int is_letter(int x);
int to_lower(int x);
int seek_char(FILE **f, int *result);
int read_value(FILE **f, Array *result, char first);
int is_arr_equal(Array str1, Array str2);
int arr_compare(Array str1, Array str2);

#endif //LABA_3_4_ARRAY_H
