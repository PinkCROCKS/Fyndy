#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double f1(double x) {
	return (log(1 + x) / x);
}

double f2(double x) {
	return exp(pow(x, 2) / -2);
}

double f3(double x) {
	return log(1 / (1 - x));
}

double f4(double x) {
	return pow(x, x);
}

double integral(double (*function) (double), double left_boarder, double right_boarder, int n) {
	double dx = (right_boarder - left_boarder) / n;
	double I = 0.00;
	for (int i = 1; i <= n; i++) {
		I += function((left_boarder + dx * i) - dx / 2) * dx;
	}
	return I;
}

double RungeMethod(double (*function) (double), double epsilon, double left_boarder, double right_boarder) {
	double t = 0.33; //так как я пользуюсь методом средних прямоугольников, то t = 1 / 3 (по методу Рунге)
	double last_value = integral(function, left_boarder, right_boarder, 2); // начальное число шагов для интегралов беру равным 2
	int n = 4;
	double current_value = integral(function, left_boarder, right_boarder, n);
	double inaccuracy = t * fabs(current_value - last_value);
	while (inaccuracy > epsilon) {
		n *= 2;
		last_value = current_value;
		current_value = integral(function, left_boarder, right_boarder, n);
		inaccuracy = t * fabs(current_value - last_value);
	}
	return current_value;
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
    if (result * sign <= 0.0) {
        return 1;
    }
    *eps = result * sign;
    return 0;
}

int GetOpts(int argc, char** argv, double* epsilon) {
    const char* procceding_option = argv[1];
    double current_number = 0.00;

    int incorrect_input = my_atof(procceding_option, epsilon);
    if (incorrect_input){
        return 1;
    }
    //printf("%f\n", sign * current_number);
    return 0;
}

int main(int argc, char** argv) {
    double epsilon;
    if (GetOpts(argc, argv, &epsilon)) {
        printf("Incorrect_output");
    }
	printf("Integral a: %f\n", RungeMethod(&f1, epsilon, 0, 1));
    printf("Integral b: %f\n", RungeMethod(&f2, epsilon, 0, 1));
    printf("Integral c: %f\n", RungeMethod(&f3, epsilon, 0, 1));
    printf("Integral d: %f\n", RungeMethod(&f4, epsilon, 0, 1));
}