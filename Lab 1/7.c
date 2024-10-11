#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

typedef enum errors {
    zero,
    file_error,
    same_files_error,
    amount_of_arguments_error,
    sign_error
} errors;

typedef enum kOpts {
    OPT_R,
    OPT_A
} kOpts;

int GetOpts(int argc, char** argv, kOpts* option) {
    int key = 1;
    for (int i = 1; i < argc; i++){
        char* procceding_option = argv[i];
        if ((key)&&(procceding_option[0] == '/' || procceding_option[0] == '-')) {
            switch (procceding_option[1]) {
            case 'r':
                *option = OPT_R;
                key = 0;
                break;
            case 'a':
                *option = OPT_A;
                key = 0;
                break;
            default:
                return 1;
            }
        }
    }
    return 0;
}

int is_errors(int result) {
    if (!(result)) {
        return 0;
    }
    switch (result) {
    case file_error:
        printf("No such file or Directory");
        return 1;
    case same_files_error:
        printf("All files must be different");
        return 1;
    case amount_of_arguments_error:
        printf("Mistake with amount of Arguments");
    }
}

int is_str_equal(char* str1, char* str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) return 0;
        str1++;
        str2++;
    }
    if (!(*str1) && !(*str2)) return 1;
    return 0;
}

int same_files(int argc, char** paths) {
    int result = 0;
    for (int i = 1; i < argc; i++) {
        result = (is_str_equal(paths[2 + i], paths[i + 1])) || result;
    }
    return result;
}

int translate_number_system(int number, char* answer, int number_system) {
    if (number < 0) {
        return sign_error;
    }
    const char hexDigits[] = "0123456789ABCDEF";
    char hexString[23]; //максимальный инт 2,147,483,647 это 1333333333333333333333 в 16ричной = 22 символов + 1 [0] = 23

    int i = 0;
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

int is_upper_case(char symbol) {
    return (symbol <= 'Z') && (symbol >= 'A');
}

char lower_case(char symbol) {
    if (is_upper_case(symbol)) {
        return symbol + ' ';
    }
    return symbol;
}

int HandlerOptR(int argc, char** paths) {
    if (!(argc == 3)) {
        return amount_of_arguments_error;
    }
    if (same_files(argc, paths)) {
        return same_files_error;
    }
    FILE* file1 = fopen(paths[2], "r");
    FILE* file2 = fopen(paths[3], "r");
    if ((file1 == NULL) || (file2 == NULL)) {
        fclose(file1);
        fclose(file2);
        return file_error;
    }
    FILE* file3 = fopen(paths[4], "w");
    char sym1[255];
    char sym2[255];
    while((fscanf(file1, "%s", sym1) != EOF)&&(fscanf(file2, "%s", sym2) != EOF)){
        fprintf(file3, "%s %s ", sym1, sym2);
    }
    if ((fscanf(file1, "%s", sym1) != EOF)) {
        fprintf(file3, "%s ", sym1);
        while (fscanf(file1, "%s", sym1) != EOF) {
            fprintf(file3, "%s ", sym1);
        }
    }
    else if ((fscanf(file2, "%s", sym2) != EOF)) {
        fprintf(file3, "%s ", sym2);
        while (fscanf(file2, "%s", sym2) != EOF) {
            fprintf(file3, "%s ", sym2);
        }
    }
    fclose(file1);
    fclose(file2);
    fclose(file3);
    return 0;
}

int HandlerOptA(int argc, char** paths) {
    if (!(argc == 2)) {
        return amount_of_arguments_error;
    }
    if (same_files(argc, paths)) {
        return same_files_error;
    }
    FILE* file1 = fopen(paths[2], "r");
    FILE* file2 = fopen(paths[3], "w");
    if (file1 == NULL) {
        return file_error;
    }
    char sym1[255];
    int k = 1;
    while (fscanf(file1, "%s", sym1) != EOF) {
        for (int i = 0; sym1[i] != '\0'; i++) {
            if (!(k % 10)) {
                int asci = (int)lower_case(sym1[i]);
                char str[23];
                translate_number_system(asci, str, 4);
                fprintf(file2, "%s", str);
            }
            else if (!(k % 5)) {
                int asci = (int)sym1[i];
                char str[12];
                translate_number_system(asci, str, 8);
                fprintf(file2, "%s", str);

            }
            else if (!(k % 2)) {
                fprintf(file2, "%c", lower_case(sym1[i]));
            }
            else {
                fprintf(file2, "%c", sym1[i]);
            }
        }
        fprintf(file2, " ");
        k++;
    }
    fclose(file1);
    fclose(file2);
    return 0;
}

int main(int argc, char** argv) {
    kOpts opt = 0;
    int exit_file = 0;
    int (*handlers[2])(int, char**) = {
        HandlerOptR,
        HandlerOptA
    };
    if (GetOpts(argc, argv, &opt)) {
        printf("Incorrect options\n");
        return 1;
    }
    int result = handlers[opt](argc - 2, argv);
    return (is_errors(result));
}