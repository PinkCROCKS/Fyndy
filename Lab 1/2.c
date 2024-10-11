#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef enum Errors {
    Invalid_input = 2
} Errors;


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
            printf("Invalid_input");
            return Invalid_input;
        }
    } 
    if (result == 0.0) {
        printf("incorrect arguments");
        return 1;
    }
    *eps = result * sign;
    return 0;
}


int GetOpts(int argc, char** argv, double* parametr) {
    if (argc > 2) {
        printf("Too much arguments\n");
        return 1;
    }
    return my_atof(argv[1], parametr);
}

double Factorial(int n){
    int answer = 1;
    for (int i = 2; i <= n; i++) {
        answer *= i;
    }

    return answer * 1.00;
}

//=======================Rows============================

double eRow(double epsilon) {
    int n = 1;
    double last_element = 0.0000;
    double current_element = 1.0000;
    while (current_element - last_element >= epsilon) {
        last_element = current_element;
        current_element += 1 / Factorial(n);
        n++;
    }
    return current_element;
}

double piRow(double epsilon) {
    int n = 2;
    double last_element = 0.0000;
    double current_element = 4.0000;
    while (fabs(current_element - last_element) >= epsilon) {
        last_element = current_element;
        current_element += 4 * (pow(-1, n - 1) / (2 * n - 1)) * 1.000;
        n++;
    }
    return current_element;
}

double logRow(double epsilon) {
    int n = 2;
    double last_element = 0.0000;
    double current_element = 1.0000;
    while (fabs(current_element - last_element) >= epsilon) {
        last_element = current_element;
        current_element += (pow(-1, n - 1) / n) * 1.000;
        n++;
    }
    return current_element;
}

double squareRow(double epsilon) {
    int k = -2;
    double last_element = 0.0000;
    double current_element = pow(2, pow(2, k));
    k--;
    while (fabs(current_element - last_element) >= epsilon) {
        last_element = current_element;
        current_element *= pow(2, pow(2, k));
        k--;
    }
    return current_element;
}

double specialRound(double n){ // Реализуем функцию поиска максимального целого непревосзодящего n
    return trunc(n);
}

//=====================Equations=======================

double eEquation(double epsilon) {
    double a = 1.00;
    double b = 5.00;
    double x;
    while (fabs(b - a) >= epsilon) {
        x = fabs(b + a) / 2;
        if (log(x) - 1 > 0) {
            b = x;
        }
        else {
            a = x;
        }
    }
    return x;
}

double piEquation(double epsilon) {
    double a = 1.00;
    double b = 4.00;
    double x;
    while (fabs(b - a) >= epsilon) {
        x = fabs(b + a) / 2; // В заданном внутри лабы промежутке, у косинуса экстремум соответсвенно этот метод не работает
        if (cos(x / 2) < 0) { // использую другой период функции косинуса так как в точке Пи пополам он непрерывно убывает
            b = x;
        }
        else {
            a = x;
        }
    }
    return x;
}

double logEquation(double epsilon) {
    double a = 0.00;
    double b = 3.00;
    double x;
    while (fabs(b - a) >= epsilon) {
        x = fabs(b + a) / 2;
        if (exp(x) - 2 > 0) {
            b = x;
        }
        else {
            a = x;
        }
    }
    return x;
}

double squareEquation(double epsilon) {
    double a = 0.00;
    double b = 3.00;
    double x;
    while (fabs(b - a) >= epsilon) {
        x = fabs(b + a) / 2;
        if (pow(x, 2) - 2 > 0) {
            b = x;
        }
        else {
            a = x;
        }
    }
    return x;
}

//=========================Limits=============================

double eLimit(double epsilon) {
    int n = 12;
    double current_element = pow((1.00 + 1.00 / n), n * 1.00) * 1.00;
    double last_element = 0;
    while (fabs(current_element - last_element) >= epsilon) {
        last_element = current_element;
        n++;
        current_element = pow((1.00 + 1.00 / n), n * 1.00) * 1.00;
    }
    return current_element;
}

double piLimit(double epsilon) {
    int n = 1;
    double current_element = pow(pow(2, n) * Factorial(n), 4) / (n * pow(Factorial(2 * n), 2));
    double last_element = 0;
    while (fabs(current_element - last_element) >= epsilon) {
        last_element = current_element;
        current_element *= (4 * n * (n + 1)) / pow(2 * n + 1, 2);
        n++;
    }
    return current_element;
}

double logLimit(double epsilon) {
    int n = 12;
    double current_element = (pow(2, 1.00 / n) - 1) * n;
    double last_element = 0;
    while (fabs(current_element - last_element) >= epsilon) {
        last_element = current_element;
        n++;
        current_element = (pow(2, 1.00 / n) - 1) * n;
    }
    return current_element;
}

double squareLimit(double epsilon) {
    double current_element = -0.5;
    double last_element = 0;
    while (fabs(last_element - current_element) >= epsilon) {
        last_element = current_element;
        current_element = last_element - pow(last_element, 2) / 2 + 1;
    }
    return current_element;
}

int main(int argc, char** argv) {
    double epsilon; 
    if (GetOpts(argc, argv, &epsilon)) {
        return 1;
    }
    printf("E: \n limit: %f; Row: %f; Equation: %f;\n", eLimit(epsilon), eRow(epsilon), eEquation(epsilon));
    printf("PI: \n limit: %f; Row: %f; Equation: %f\n", piLimit(epsilon), piRow(epsilon), piEquation(epsilon));
    printf("LOG: \n limit: %f; Row: %f; Equation: %f\n", logLimit(epsilon), logRow(epsilon), logEquation(epsilon));
    printf("SQUARE: \n limit: %f; Row: %f; Equation: %f\n", squareLimit(epsilon), squareRow(epsilon), squareEquation(epsilon));
	return 0;
}