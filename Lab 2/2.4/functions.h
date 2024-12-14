//
// Created by Ярослав on 26.11.2024.
//

#ifndef INC_2_4_FUNCTIONS_H
#define INC_2_4_FUNCTIONS_H

#include "return_code.h"

#define EPS 1e-9

typedef struct {
    double x;
    double y;
} Point;

enum return_code is_convex(int* result, int count, ...);
enum return_code evaluate_polynomial(double* result, double x, int degree, ...);
enum return_code find_kaprekar_nums(int** massive_of_ans, int base, int count, ...);

#endif //INC_2_4_FUNCTIONS_H
