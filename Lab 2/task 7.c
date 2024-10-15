#include <stdio.h>
#include <stdio.h>
#include <math.h>

long double calculate_dichotomy(long double (*func)(long double x), long double a, long double b, long double eps) {
    int sign = 1;
    if (func(a) > func(b)){
        sign = 0;
    }
    long double c = (a + b) / 2;
    long double last;
    while (fabsl(b - a) >= eps) {
        if ((func(c) > 0)&&(sign)) {
            b = c;
        } else if (func(c) > 0){
            a = c;
        } else if (sign){
            a = c;
        } else {
            b = c;
        }
        c = (a + b) / 2;
    }
    return c;
}

long double func1(long double x){
    return cos((x/2));
}

int main(){
    long double a = 1;
    long double b = 4;
    long double epsilon = 0.0001;
    printf("answer = %Lf", calculate_dichotomy(&func1, a, b, epsilon));
    return 0;
}