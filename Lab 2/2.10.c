#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

typedef enum {
    OK,
    INPUT_ERROR,
    MEMORY_ERROR
} kOpt;

void derivate(int degree, double *coeffs) {
    for (int i = 0; i < degree; i++) {
        coeffs[i] = coeffs[i + 1] * (i + 1);
    }
}

kOpt solve_polynomial(double* res, double point, int degree, const double *coefs) {
    *res = 0;
    double cur_degree = 1.0;
    for (int i = 0; i < degree + 1; i++) {
        *res += coefs[i] * cur_degree;
        cur_degree *= point;
    }
    return OK;
}

kOpt solve_coefs(double a, double** coefs_g, int* size_coefs, int degree, double epsilon, ...) {
    if (degree < 0) {
        return INPUT_ERROR;
    }

    va_list ptr;
    va_start(ptr, epsilon);
    double * yu = (double*)malloc((degree + 1) * sizeof(double));
    if (yu == NULL) {
        return MEMORY_ERROR;
    }
    *coefs_g = yu;

    double *coefs = (double*)malloc((degree + 1) * sizeof(double));
    if (coefs == NULL){
        free(*coefs_g);
        return MEMORY_ERROR;
    }
    for (int i = 0; i < degree + 1; i++) {
        coefs[i] = va_arg(ptr, double);
    }
    va_end(ptr);

    double factorial_tmp = 1;
    for (int i = 0; i < degree + 1; i++) {
        double coefficient_g;
        solve_polynomial(&coefficient_g, a, degree - i, coefs);

        if (fabs(coefficient_g) < epsilon) {
            coefficient_g = 0.0;
        }

        (*coefs_g)[i] = coefficient_g;

        if (i > 1) {
            factorial_tmp *= i;
            (*coefs_g)[i] /= factorial_tmp;
        }

        derivate(degree - i, coefs);
    }

    *size_coefs = degree + 1;
    free(coefs);
    return OK;
}

int main() {
    int i = 0;
    printf("%d", i);
    double* coefs_g = NULL;
    int size_coefs;
    int degree = 1;
    double epsilon = 1e-6;
    double a = 1.0;

    switch (solve_coefs(a, &coefs_g, &size_coefs, degree, epsilon, 1.0, 2.0)) {
        case OK:
            printf("f(x) = ");
            for (int i = 0; i < degree + 1; i++) {
                if (i == 0) {
                    printf("%f", coefs_g[i]);
                } else {
                    printf(" + %.2f*(x - %.2f)^%d", coefs_g[i], a , i);
                }
            }
            printf("\n");
            break;
        case INPUT_ERROR:
            fprintf(stderr, "Invalid parameter.\n");
            break;
        case MEMORY_ERROR:
            fprintf(stderr, "Error malloc memory\n");
            break;
    }

    free(coefs_g);
    return 0;
}
