#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef enum erors {
    zero_eror,
    sign_eror,
    function_with_table,
    big_number_eror
} erors;

typedef enum kOpts {
    OPT_H,
    OPT_P,
    OPT_S,
    OPT_E,
    OPT_A,
    OPT_F
} kOpts;

int HandlerOptH(int number) {
    if (number == 0) {
        return zero_eror;
    }
    for (int i = 1; i <= 100; ++i) {
        if (!(i % number)) {
            printf("%d\n", i);
        }
    }
}

int HandlerOptP(int number, char* answer) {
    if (number == 1) {
        char* prefix = "not simple and not composite";
        for (int i = 0; i <= sizeof("not simple and not composite"); i++) {
            answer[i] = prefix[i];
        }
        return 0;
    }
    int flag = 1;
    for (int i = 2; i <= sqrt(number); ++i) {
        if (!(number % i)) {
            flag = 0;
        }
    }

    if (flag) {
        char* prefix = "simple";
        for (int i = 0; i <= sizeof("not simple and not composite"); i++) {
            answer[i] = prefix[i];
        }
        return 0;
    }
    else {
        char* prefix = "not simple";
        for (int i = 0; i <= sizeof("not simple and not composite"); i++) {
            answer[i] = prefix[i];
        }
        return 0;
    }
}

int HandlerOptS(int number, char* answer) {
    if (number < 0) {
        return sign_eror;
    }
    const char hexDigits[] = "0123456789ABCDEF";
    char hexString[10]; //максимальный инт 2,147,483,647 это 7FFFFFFF в 16ричной = 9 символов + 1 [0] = 10

    int i = 0;
    int number_system = 16;
    if (number == 0) {
        hexString[i++] = '0';
    }
    else {
        while (number > 0) {
            int remainder = number % number_system;
            hexString[i++] = hexDigits[remainder];
            number /= number_system;
        }
        int k = 0;
        for (int j = i - 1; j >= 0; --j) {
            answer[k++] = hexString[j];
        }
        answer[k] = '\0';
    }
    return 0;

}

int HandlerOptE(int number, char* answer) {
    if (number > 10) {
        return big_number_eror;
    }
    for (long long int i = 1; i <= 10; ++i) {
        long long int k = i;
        for (long long int j = 1; j <= number; ++j) {
            printf("%llu ", k);
            k *= i;
        }
        printf("\n");
    }
    return function_with_table;
}

int HandlerOptA(int number, char* answer) {
    int summ = ((1 + number) * number) / 2;
    int i = 0;
    char str[40];
    while (summ > 0) {
        str[i] = (summ % 10) + '0';
        i++;
        summ /= 10;
    }
    answer[i] = '\0';
    int k = 0;
    for (int j = i - 1; j >= 0; j--) {
        answer[j] = str[k++];
    }
    return 0;
}

int HandlerOptF(int n, char* answer) {
    if (n > 20) {
        return big_number_eror;
    }
    long long int factorial = 1;
    for (int i = 2; i <= n; i++) {
        factorial *= i;
    }
    int i = 0;
    char str[40];
    while (factorial > 0) {
        str[i] = (factorial % 10) + '0';
        i++;
        factorial /= 10;
    }
    answer[i] = '\0';
    int k = 0;
    for (int j = i - 1; j >= 0; j--) {
        answer[j] = str[k++];
    }
    return 0;
}

int GetOpts(int argc, char** argv, kOpts* option, int* number) {
    int amount_of_keys = 0;
    if (argc != 3) {
        return 1;
    }

    for (int i = 1; i <= 2; ++i) {
        const char* procceding_option = argv[i];
        if ((procceding_option[0] == '/' || procceding_option[0] == '-')&&(amount_of_keys == 0)) {
            amount_of_keys++;
            switch (procceding_option[1]) {
            case 'h':
                *option = OPT_H;
                break;
            case 'p':
                *option = OPT_P;
                break;
            case 's':
                *option = OPT_S;
                break;
            case 'e':
                *option = OPT_E;
                break;
            case 'a':
                *option = OPT_A;
                break;
            case 'f':
                *option = OPT_F;
                break;
            default:
                return 1;
            }
            if (procceding_option[2] != '\0') {
                return 1;
            }
        }
        else if (procceding_option[0] == '/' || procceding_option[0] == '-') {
            return 1;
        }
        else {
            for (int j = 0; procceding_option[j]; ++j) {
                if (procceding_option[j] >= '0' && procceding_option[j] <= '9') {
                    *number *= 10;
                    *number += procceding_option[j] - '0';
                }
                else {
                    return 1;
                }
            }
        }
    }
    if (amount_of_keys == 0) {
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    kOpts opt = 0;
    int procceed_number = 0;
    char answer[45];
    int (*handlers[6])(int, char*) = {
        HandlerOptH,
        HandlerOptP,
        HandlerOptS,
        HandlerOptE,
        HandlerOptA,
        HandlerOptF
    };

    if (GetOpts(argc, argv, &opt, &procceed_number)) {
        printf("Incorrect arguments\n");
        return 1;
    }

    int correct_work = handlers[opt](procceed_number, &answer);
    if (!(correct_work)) {
        printf("%s\n", answer);
    }
    else {
        switch (correct_work)
        {
        case zero_eror:
            printf("can't divide on zero");
            break;
        case sign_eror:
            printf("your number can't be < 0");
            break;
        case function_with_table:
            printf("Table");
            break;
        case big_number_eror:
            printf("too big number");
            break;
        }
        return 0;
    }
}