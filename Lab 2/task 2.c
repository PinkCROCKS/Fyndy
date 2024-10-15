#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

double middleGeometry(int amount, ...){
    va_list factor;
    va_start(factor, amount);
    double composition = 1.0;
    for (int i = 0; i < amount; i++){
        double n = va_arg(factor, double);
        composition *= n;
    }
    va_end(factor);
    double cast = 1 / (1.00 * amount);
    return powl(composition, cast);
}

long double fastPower(long double base, int k) {
    if (k == 0) {
        return 1.0;
    }
    if (k < 0) {
        return 1.0 / fastPower(base, -k);
    }
    if (k % 2 == 0) {
        long double temp = fastPower(base, k / 2);
        return temp * temp;
    }
    return base * fastPower(base, k - 1);
}

int main() {
    printf("%lf", middleGeometry(2, 81.192, 15.89));
    printf("%.10Lf", fastPower(2, -25));
    return 0;
}
