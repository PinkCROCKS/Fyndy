#ifndef SOLUTION_H
#define SOLUTION_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <limits.h>

typedef enum Errors {
    Invalid_input = 2,
    Memory_leak
} Errors;

typedef struct Vector {
    int n;
    double* coords;
} Vector;

int generate_positive_definite_matrix(int n, double **P);

double norm1(Vector* v, void* args);

double norm2(Vector* v, void* args);

double norm3(Vector* v, void* args);

int Allocate(double*** norms, Vector*** vectors, int n_vectors, int n_norms, int n, double*** A, char*** result);

void delete(double*** norms, Vector*** vectors, int n_vectors, int n_norms, int n, double*** A, char*** result);

void calculate_norms(Vector** vectors, double** norms, int n_vectors, int n_norms, ...);

void max_norm(Vector** vectors, double** norms, int n_vectors, int n_norms, char** result);

#endif