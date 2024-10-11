#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double Factorial(int n) {
    double answer = 1.0;
    for (int k = 1; k <= n; k++) {
        answer *= k;
    }
    return answer;
}

double secondFactorial(int n) {
    double answer = 1.00;
    if (n % 2) {
        for (int k = 1; k <= n; k += 2) {
            answer *= k;
        }
        return answer;
    }
    for (int k = 2; k <= n; k += 2) {
        answer *= k;
    }
    return answer;
}

int summA(double* parametrs, double* answer) {
    double summ = 0;
    double n = 0;
    double epsilon = parametrs[0];
    double x = parametrs[1];
    double current_element = 1;
    while (current_element > epsilon) {
        summ += current_element;
        n++;
        current_element *= x / n;
    }
    *answer = summ;
    return 0;
}

int summB(double* parametrs, double* answer) {
    double epsilon = parametrs[0];
    double x = parametrs[1];
    double summ = 0;
    double n = 0;
    double current_element = 1;
    while (fabs(current_element) > epsilon) {
        n++;
        summ += current_element;
        current_element *= -1 * x * x / (2 * n * (2 * n - 1));
    }
    *answer = summ;
    return 0;
}

int summC(double* parametrs, double* answer) {
    double epsilon = parametrs[0];
    double x = parametrs[1];
    if (fabs(x) >= 1) {
        return 2;
    }
    int n = 0;
    double current_element = 1;
    double summ = 0;
    while (fabs(current_element) > epsilon) {
        n++;
        summ += current_element;
        current_element = (current_element * 27 * n * n * n * x * x) / 
            (3 * n * (3 * n - 1) * (3 * n - 2));
    }
    *answer = summ;
    return 0;
}

int summD(double* parametrs, double* answer) {
    double epsilon = parametrs[0];
    double x = parametrs[1];
    if (fabs(x) >= 1) {
        return 2;
    }
    int n = 1;
    double last_element = 0;
    double current_element = -1 * x * x / 2;
    double summ = current_element;
    while ((fabs(current_element - last_element) > epsilon)) {
        last_element = current_element;
        current_element = (last_element * pow(x, 2) * (2 * n + 1) * -1) / (2 * n + 2);
        summ += current_element;
        n++;
    }
    *answer = summ;
    return 0;
}

int my_atof(const char* string, double* eps) {
    int sign = 1, signs = 0, dots = 0, numbers = 0;
    double result = 0.0, fraction = 1.0;
    for (int i = 0; string[i] != '\0'; ++i) {
        if (string[i] == '-' && signs == 0) {
            signs = 1;
            sign = -1;
        }
        else if (string[i] == '.' && dots == 0) {
            dots = 1;
        }
        else if (string[i] >= '0' && string[i] <= '9' && dots == 0) {
            result += result * 10.0 + (string[i] - '0');
        }
        else if (string[i] >= '0' && string[i] <= '9' && dots == 1) {
            fraction /= 10.0;
            result += (string[i] - '0') * fraction;
        }
        else {
            return 1;
        }
    }
    *eps = result * sign;
    return 0;
}

int GetOpts(int argc, char** argv, double* parametrs) {
    char* procceding_option;
    int incorrect_input;
    for (int i = 0; i < 2; i++) {
        procceding_option = argv[i + 1];
        incorrect_input = my_atof(procceding_option, &parametrs[i]);
        if (incorrect_input) {
            return 1;
        }
    }
    if (parametrs[0] <= 0.0) {
        return 1;
    }
    //printf("%f\n", sign * current_number);
    return 0;
}

int main(int argc, char** argv) {
    double parametrs[2];
    if (GetOpts(argc, argv, parametrs)) {
        printf("Incorrect options\n");
        return 1;
    }
    printf("%f, %f\n", parametrs[0], parametrs[1]);
    double summ;
    if (!(summA(parametrs, &summ))) {
        printf("A: %f\n", summ);
    }
    if (!(summB(parametrs, &summ))) {
        printf("B: %f\n", summ);
    }
    if (!(summC(parametrs, &summ))) {
        printf("C: %f\n", summ);
    }
    else {
        printf("C: don`t converges\n");
    }
    if (!(summD(parametrs, &summ))) {
        printf("D: %f\n", summ);
    }
    else {
        printf("D: don`t converges\n");
    }
    return 0;
}