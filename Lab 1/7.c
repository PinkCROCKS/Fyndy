#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)

typedef enum errors {
    zero,
    file_error,
    same_files_error,
    amount_of_arguments_error,
    sign_error,
    realpath_error,
    memory_error
} errors;

typedef enum kOpts {
    OPT_R,
    OPT_A
} kOpts;

int same_files(const char* path1, const char* path2) {
    char* real_path1 = realpath(path1, NULL);
    if (real_path1 == NULL) {
        return realpath_error;
    }

    char* real_path2 = realpath(path2, NULL);
    if (real_path2 == NULL) {
        return realpath_error;
    }

    int result = (strcmp(real_path1, real_path2) == 0);

    free(real_path1);
    free(real_path2);

    return result;
}

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
        case realpath_error:
            printf("Mistake with file in realpath");
    }
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

int is_separation(char c){
    return (c == ' ') || (c == '\n') || (c == '\t') || (c == '\r');
}

void file_reader(FILE* file1, FILE* file2, FILE* file3){
    int k = 0;
    int resume1 = 1;
    int resume2 = 1;
    char last = ' ';
    while (resume1 || resume2){
        if (!k){
            char c = getc(file1);
            if (c == EOF){
                k = 1;
                resume1 = 0;
            }
            else if ((is_separation(c))&&(!(is_separation(last)))){
                fprintf(file3, " ");
                last = ' ';
                if (resume2){
                    k = 1;
                }
            }
            else if (is_separation(c)){
                continue;
            }
            else {
                fprintf(file3, "%c", c);
                last = c;
            }
        }
        else {
            char c = getc(file2);
            if (c == EOF){
                k = 0;
                resume2 = 0;
            }
            else if ((is_separation(c))&&(!(is_separation(last)))){
                fprintf(file3, " ");
                last = ' ';
                if (resume1){
                    k = 0;
                }
                k = 0;
            }
            else if (is_separation(c)){
                continue;
            }
            else {
                fprintf(file3, "%c", c);
                last = c;
            }
        }
    }
}

int modify_char(FILE* file2, int k, char c) {
    printf("k = %d\n", k);
    if (k % 10 == 0) {
        int asci = (int) lower_case(c);
        char str[23];
        translate_number_system(asci, str, 4);
        fprintf(file2, "%s", str);
        return 0;
    }
    if (!(k % 5)) {
        int asci = (int) c;
        char str[12];
        translate_number_system(asci, str, 8);
        fprintf(file2, "%s", str);
        return 0;
    }
    if (!(k % 2)) {
        fprintf(file2, "%c", lower_case(c));
        return 0;
    }
    fprintf(file2, "%c", c);
    return 0;
}

int HandlerOptR(int argc, char** paths){
    if (argc != 3){
        return amount_of_arguments_error;
    }
    FILE * file1 = fopen(paths[2], "r");
    if (file1 == NULL){
        return file_error;
    }
    if (same_files(paths[2], paths[3])) {
        return same_files_error;
    }
    FILE * file2 = fopen(paths[3], "r");
    if (file2 == NULL){
        return file_error;
    }
    if (same_files(paths[3], paths[4])) {
        return same_files_error;
    }
    if (same_files(paths[2], paths[4])) {
        return same_files_error;
    }
    FILE * file3 = fopen(paths[4], "w");
    file_reader(file1, file2, file3);
    fclose(file1);
    fclose(file2);
    fclose(file3);
    return 0;
}

int HandlerOptA(int argc, char** paths) {
    if (!(argc == 2)) {
        return amount_of_arguments_error;
    }
    FILE* file1 = fopen(paths[2], "r");
    if (file1 == NULL) {
        printf("MYAY");
        return file_error;
    }
    if (same_files(paths[2], paths[3])) {
        return same_files_error;
    }
    FILE* file2 = fopen(paths[3], "w");
    char c = getc(file1);
    char last = ' ';
    int k = 0;
    while (c != EOF){
        if ((is_separation(c))&&(!(is_separation(last)))){
            fprintf(file2, " ");
            k++;
        }
        else if (is_separation(c)){
            last = c;
            c = getc(file1);
            continue;
        }
        else {
            modify_char(file2, k + 1, c);
        }
        last = c;
        c = getc(file1);
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