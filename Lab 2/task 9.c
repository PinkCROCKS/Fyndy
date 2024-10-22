#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

int is_prime(long int number){
    int k = 2;
    while (k * k <= number){
        if (number % k == 0){
            return 0;
        }
        k++;
    }
    return 1;
}

int prod_prime_devisors(int number){
    if (is_prime(number)){
        return number;
    }
    int answer = 1;
    int i = 2;
    while (i * i < number){
        if ((number % i == 0)&&(is_prime(i))){
            answer *= i;
        }
        if ((number % i == 0)&&(is_prime(number / i))){
            answer *= number / i;
        }
        i++;
    }
    if ((i * i == number)&&(is_prime(i))){
        answer *= i;
    }
    return answer;
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void reduce(int *numerator, int *denominator) {
    int divisor = gcd(*numerator, *denominator);
    *numerator /= divisor;
    *denominator /= divisor;
}

int validate_denominator(double x, int* numerator, int* denominator) {
    *numerator = 1;
    *denominator = (int)round(1.0 / (x));

    double eps = 0.00001;
    double approximation = (double)*numerator / *denominator;
    double difference = approximation - x;

    while (fabs(difference) >= eps) {
        if (difference > 0) {
            int diffDenominator = (int)round(1.0 / difference);
            int newNumerator = *numerator * diffDenominator - *denominator;
            int newDenominator = *denominator * diffDenominator;
            *numerator = newNumerator;
            *denominator = newDenominator;
        } else {
            int diffDenominator = (int)round(1.0 / -difference);
            int newNumerator = *numerator * diffDenominator + *denominator;
            int newDenominator = *denominator * diffDenominator;
            *numerator = newNumerator;
            *denominator = newDenominator;
        }

        approximation = (double)*numerator / *denominator;
        difference = approximation - x;
    }
    reduce(numerator, denominator);
    return 0;
}

void copy_string(char* dest, const char* src) {
    while ((*dest++ = *src++));
}

int is_ended_in_base(char** answer, int base, int amount, ...) {
    va_list numbers;
    if (amount == 0) {
        answer[0] = (char*)malloc(12 * sizeof(char));
        copy_string(answer[0], "No Numbers");
        return 0;
    }
    answer[0] = (char*)malloc(9 * sizeof(char));
    copy_string(answer[0], "answer\n");
    double epsilon = 1e-16;
    va_start(numbers, amount);
    for (int i = 0; i < amount; i++) {
        double current = va_arg(numbers, double);
        if ((current > 1.0) || (current < 0.0)) {
            answer[i + 1] = (char*)malloc(17 * sizeof(char));
            copy_string(answer[i + 1], "Incorrect input\n");
            continue;
        }
        int nominator;
        int denominator;
        validate_denominator(current, &nominator, &denominator);
        int prod = prod_prime_devisors(denominator);
        if (base % prod == 0) {
            answer[i + 1] = (char*)malloc(5 * sizeof(char));
            copy_string(answer[i + 1], "Yes\n");
        } else {
            answer[i + 1] = (char*)malloc(4 * sizeof(char));
            copy_string(answer[i + 1], "No\n");
        }
    }
    va_end(numbers);
    return 0;
}

int main() {
    int amount = 5;
    char* answer[16];
    is_ended_in_base(answer, 8, amount, 0.875, 0.125, 0.375, 0.5, 0.04);
    for (int i = 0; i < amount + 1; i++) {
        printf("%s", answer[i]);
        free(answer[i]);
    }
    return 0;
}
