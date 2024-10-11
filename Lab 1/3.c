#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef enum Errors {
    invalid_input = 2
} Errors;

typedef enum kOpts {
    OPT_Q,
    OPT_M,
    OPT_T
} kOpts;

int compareNumbers(double epsilon, double a, double b) {
    if (fabs(a - b) <= epsilon) {
        return 1;
    }
    return 0;
}

int my_atof2(const char* string, double* eps) {
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
            return invalid_input;
        }
    }
    if (compareNumbers(0.0000000001, result, 0.0)) {
        printf("incorrect arguments");
        return 1;
    }
    *eps = result * sign;
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
            return invalid_input;
        }
    }
    if (compareNumbers(0.0000000001, result, 0.0)) {
        printf("incorrect arguments");
        return 1;
    }
    *eps = result * sign;
    return 0;
}


int GetOpts(int argc, char** argv, kOpts* option, double* parametrs) {
    int key = 0;
    for (int i = 1; i < argc; i++) {
        const char* procceding_option = argv[i];
        if ((procceding_option[0] == '/' || procceding_option[0] == '-')&&(procceding_option[2] == '\0')) {
            key++;
            switch (procceding_option[1]) {
            case 'q':
                *option = OPT_Q;
                break;
            case 'm':
                *option = OPT_M;
                break;
            case 't':
                *option = OPT_T;
                break;
            default:
                return 1;
            }
        }
        else if (key == 1) {
            if (i - 2 > 3) {
                printf("To much arguments\n");
                return 1;
            }
            if (*option == OPT_M) {
                int incorrect_input = my_atof(argv[i], &parametrs[i - 2]);
                if (incorrect_input) {
                    return 1;
                }
            }
            else {
                int incorrect_input;
                if (i == 2) {
                    incorrect_input = my_atof2(argv[i], &parametrs[i - 2]);
                }
                else {
                    incorrect_input = my_atof(argv[i], &parametrs[i - 2]);
                }
                if (incorrect_input) {
                    return 1;
                }
            }
            //printf("%f\n", sign * current_number);
        }
        else {
            return invalid_input;
        }
    }
    return 0;
}

void solutionsOfEqual(double epsilon, double A, double B, double C) {
    double D = B * B - 4 * A * C;
    if (compareNumbers(epsilon, A, 0.00)&&compareNumbers(epsilon, B, 0.00)&&compareNumbers(epsilon, C, 0.00)) {
        printf("Infinity solutions\n\n");
        return;
    }
    if (compareNumbers(epsilon, A, 0.00) && compareNumbers(epsilon, B, 0.00)) {
        printf("No solutions\n\n");
        return;
    }
    if (compareNumbers(epsilon, A, 0.00)) {
        printf("x = %f\n\n", -1 * (C / B));
        return;
    }
    if (D > 0) {
        double x1 = (-B + sqrt(D)) / (2 * A);
        double x2 = (-B - sqrt(D)) / (2 * A);
        printf("x1 = %f, x2 = %f\n\n", x1, x2);
        return;
    }
    if (compareNumbers(epsilon, D, 0.00)) {
        double x = (B * -1.00 / (2 * A));
        printf("x1 = x2 = %f\n\n", x);
        return;
    }
    printf("No solutions. Discriminant < 0\n\n");
}

int findUniq(double epsilon, double A, double B, double C) {
    if (compareNumbers(epsilon, A, B)) {
        return 3;
    }
    if (compareNumbers(epsilon, A, C)) {
        return 2;
    }
    return 1;
}

int* findSame(int Uniq) {
    int answer[2];
    if (Uniq == 1) {
        answer[0] = 2;
        answer[1] = 3;
        return answer;
    }
    if (Uniq == 2) {
        answer[0] = 1;
        answer[1] = 3;
        return answer;
    }
    answer[0] = 1;
    answer[1] = 2;
    return answer;
}

int allDifferent(double epsilon, double A, double B, double C) {
    if (compareNumbers(epsilon, A, B)) {
        return 0;
    }
    if (compareNumbers(epsilon, A, C)) {
        return 0;
    }
    if (compareNumbers(epsilon, B, C)) {
        return 0;
    }
    return 1;
}

void HandlerOptQ(double* parametrs, int argc) {
    if (!(argc == 4)) {
        printf("Oh no you have mistake with amount of arguments");
        return;
    }
    double epsilon = parametrs[0];
    if ((compareNumbers(epsilon, parametrs[1], parametrs[2])) && (compareNumbers(epsilon, parametrs[2], parametrs[3]))) {
        solutionsOfEqual(epsilon, parametrs[1], parametrs[2], parametrs[3]);
    }
    else if (allDifferent(epsilon, parametrs[1], parametrs[2], parametrs[3])) {
        for (int i = 1; i < 4; i++) {
            for (int j = 1; j < 4; j++) {
                for (int k = 1; k < 4; k++) {
                    if ((i != j) && (j != k) && (i != k)) {
                        printf("A: %f; B: %f; C: %f\n", parametrs[i], parametrs[j], parametrs[k]);
                        solutionsOfEqual(epsilon, parametrs[i], parametrs[j], parametrs[k]);
                    }
                }
            }
        }
    }
    else {
        int uniq = findUniq(epsilon, parametrs[1], parametrs[2], parametrs[3]);
        int same1 = findSame(uniq)[0];
        int same2 = findSame(uniq)[1];
        printf("A: %f; B: %f; C: %f\n", parametrs[uniq], parametrs[same1], parametrs[same2]);
        solutionsOfEqual(epsilon, parametrs[uniq], parametrs[same1], parametrs[same2]);
        printf("A: %f; B: %f; C: %f\n", parametrs[same1], parametrs[uniq], parametrs[same2]);
        solutionsOfEqual(epsilon, parametrs[same1], parametrs[uniq], parametrs[same2]);
        printf("A: %f; B: %f; C: %f\n", parametrs[same1], parametrs[same2], parametrs[uniq]);
        solutionsOfEqual(epsilon, parametrs[same1], parametrs[same2], parametrs[uniq]);
    }
}

void HandlerOptM(double* parametrs, int argc) {
    if (!(argc == 2)) {
        printf("Oh no you have mistake with amount of arguments");
        return;
    }
    if (!((parametrs[0] == floor(parametrs[0])) && (parametrs[1] == floor(parametrs[1])))) {
        printf("numbers in this key must be integer");
        return;
    }
    int A = (int)parametrs[0];
    int B = (int)parametrs[1];
    if (A % B == 0) {
        printf("first is divided into second");
        return;
    }
    printf("first is NOT divided into second");
}

int TriangleSides(double epsilon, double A, double B, double C) {
    if ((compareNumbers(epsilon, (A * A + B * B), C * C))) {
        return 1;
    }
    if (compareNumbers(epsilon, (A * A + C * C), B * B)) {
        return 1;
    }
    if (compareNumbers(epsilon, (B * B + C * C), A * A)) {
        return 1;
    }
    return 0;
}

void HandlerOptT(double* parametrs, int argc) {
    if (argc != 4) {
        printf("Oh no you have mistake with amount of arguments");
        return;
    }
    if (TriangleSides(parametrs[0], parametrs[1], parametrs[2], parametrs[3])) {
        printf("this parametrs can be sides of the right-angled triangle");
        return;
    }
    printf("this parametrs can NOT be sides of the right-angled triangle");
}

int main(int argc, char** argv) {
    kOpts opt = 0;
    double parametrs[4];
    double (*handlers[3])(double*, int) = {
        HandlerOptQ,
        HandlerOptM,
        HandlerOptT
    };

    if (GetOpts(argc, argv, &opt, parametrs)) {
        printf("Incorrect options\n");
        return 1;
    }
    printf("%d\n", compareNumbers(0.001, 2.0, 2.0));
    handlers[opt](parametrs, argc - 2); // 2 аргумента это файл и ключ,а внутрь функции мы передаём только

    return 0;
}