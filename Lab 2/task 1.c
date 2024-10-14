#include <stdio.h>
#include <stdlib.h>

typedef enum errors {
    zero,
    file_error,
    same_files_error,
    amount_of_arguments_error,
    sign_error,
    input_error,
    memory_error
} errors;

typedef enum kOpts {
    OPT_L,
    OPT_R,
    OPT_U,
    OPT_N,
    OPT_C
} kOpts;

int put_to_Array(char** array, char c, int* size, int *i) {
    if (*i < *size) {
        (*array)[*i] = c;
        (*i)++;
        return 0;
    }
    else {
        *size *= 2;
        int m = *size;
        char* yu = (char*)realloc(*array, *size);
        if (yu == NULL) {
            free(*array);
            return memory_error;
        }
        *array = yu;
        (*array)[*i] = c;
        (*i)++;
        return 0;
    }
}

int delete_from_int_array(int * array, int k, int* size){
    if (k == ((*size) - 1)){
        *size--;
        return 0;
    }
    for (int i = k; i < *size;i++){
        array[i] = array[i + 1];
    }
    *size--;
}

int reverse(char* array, int size){
    int i = size - 1;
    int m = size / 2;
    for (int j = 0; j < m; j++, i--){
        char t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
    return 0;
}

int concatenate(char** answer, int* size, int*k, char* str){
    for (int i = 0; str[i] !='\0'; i++){
        int result = put_to_Array(answer, str[i], size, k);
        if (result){
            return result;
        }
    }
    return 0;
}

int int_to_str(int l, char ** answer, int* size){
    int number = l;
    int i = 0;
    while (number > 0){
        char c = (number % 10) + '0';
        number /= 10;
        int result = put_to_Array(answer, c, size, &i);
        if (result){
            return result;
        }
    }
    reverse(*answer, i);
    return 0;
}

int push_error(int key) {
    if (!(key)) {
        printf("No mistakes");
        return 0;
    }
    switch (key) {
        case file_error:
            printf("No such file or Directory");
            break;
        case same_files_error:
            printf("All files must be different");
            break;
        case amount_of_arguments_error:
            printf("You have mistake with amount of arguments");
            break;
        case input_error:
            printf("Unknown symbol");
            break;
    }
    return 1;
}

int analyze_size(char* str) {
    int k = 0;
    while (str[k] != '\0') {
        k++;
    }
    return k;
}

int is_digit(char c){
    return (c <= '9')&&(c >= '0');
}

int is_lower(char c){
    return ((c <= 'z')&&(c >= 'a'));
}

int is_upper(char c){
    return ((c <= 'Z')&&(c >= 'A'));
}

int is_letter(char c){
    return ((is_upper(c))||(is_lower(c)));
}

int is_unusual_symbol(char c){
    return (!((is_digit(c))||(is_letter(c))));
}

char to_upper(char c){
    char answer;
    if (is_lower(c)){
        answer = c - ' ';
        return answer;
    }
    return c;
}

int construct_str(char* str, char** answer, int* size, int * k, int (*func)(char)){
    int i = 0;
    while (str[i] != '\0'){
        if (func(str[i])){
            int result = put_to_Array(answer, str[i], size, k);
            if (result){
                return result;
            }
        }
        i++;
    }
    return 0;
}

int my_atoi(char* str, unsigned int * number){
    unsigned int k = 0;
    for (int i = 0; str[i] != '\0';i++){
        if (!(is_digit(str[i]))){
            return input_error;
        }
        k *= 10;
        k += str[i] - '0';
    }
    *number = k;
    return 0;
}

int GetOpts(int argc, char** argv, kOpts* option) {
    int key = 1;
    char* procceding_option = argv[1];
    if ((procceding_option[0] == '/' || procceding_option[0] == '-')&&(procceding_option[2] == '\0')) {
        switch (procceding_option[1]) {
            case 'l':
                *option = OPT_L;
                key = 0;
                break;
            case 'u':
                *option = OPT_U;
                key = 0;
                break;
            case 'n':
                *option = OPT_N;
                key = 0;
                break;
            case 'r':
                *option = OPT_R;
                key = 0;
                break;
            case 'c':
                *option = OPT_C;
                key = 0;
                break;
            default:
                return input_error;
        }
    }
    else{
        return input_error;
    }
    return 0;
}

int HandlerOptL(int argc, char** argv, char** answer, int* size){
    char* str = argv[2];
    if (argc != 3){
        return amount_of_arguments_error;
    }
    int i = analyze_size(str);
    return int_to_str(i, answer, size);
}

int HandlerOptR(int argc, char** argv, char** answer, int* size){
    if (argc != 3){
        return amount_of_arguments_error;
    }
    int i = 0;
    while (argv[2][i] != '\0'){
        int result = put_to_Array(answer, argv[2][i], size, &i);
        if (result){
            return result;
        }
    }
    int result = reverse(*answer, i);
    return result;
}

int HandlerOptU(int argc, char** argv, char** answer, int* size){
    if (argc != 3){
        return amount_of_arguments_error;
    }
    int i = 0;
    while (argv[2][i] != '\0'){
        if (i % 2 == 0){
            int result = put_to_Array(answer, argv[2][i], size, &i);
            if (result){
                return result;
            }
        }
        else{
            char c = to_upper(argv[2][i]);
            int result = put_to_Array(answer, c, size, &i);
            if (result){
                return result;
            }
        }
    }
}

int HandlerOptN(int argc, char** argv, char** answer, int* size){
    if (argc != 3){
        return amount_of_arguments_error;
    }
    int k = 0;
    int result = construct_str(argv[2], answer, size, &k, &is_digit);
    if (result){
        return result;
    }
    result = construct_str(argv[2], answer, size, &k, &is_letter);
    if (result){
        return result;
    }
    result = construct_str(argv[2], answer, size, &k, &is_unusual_symbol);
    if (result){
        return result;
    }
    return 0;
}

int HandlerOptC(int argc, char** argv, char** answer, int* size){
    int filled_size = argc;
    if (argc <= 4){
        return amount_of_arguments_error;
    }
    unsigned int n;
    int result = my_atoi(argv[3], &n);
    if (result){
        return result;
    }
    int k = 0;
    while (argv[2][k] != '\0'){
        result = put_to_Array(answer, argv[2][k], size, &k);
    }
    srand(n);
    int indexes[argc - 4];
    int m = argc - 4;
    for (int i = 0; i < argc - 4; i++){
        indexes[i] = 4 + i;
    }
    for (int i = 0; i < argc - 4;i++){
        int number_str = rand() % (argc - 4 - i);
        result = concatenate(answer, size, &k, argv[indexes[number_str]]);
        delete_from_int_array(indexes, number_str, &m);
        if (result){
            return result;
        }
    }
    return 0;
}

int main(int argc, char** argv){
    kOpts opt = 0;
    int (*handlers[5])(int, char**, char**, int *) = {
            HandlerOptL,
            HandlerOptR,
            HandlerOptU,
            HandlerOptN,
            HandlerOptC
    };
    int result = GetOpts(argc, argv, &opt);
    if (result){
        return push_error(result);
    }
    int size = 2;
    char* answer = (char *)malloc(size);
    if (answer == NULL){
        return memory_error;
    }
    result = handlers[opt](argc, argv, &answer, &size);
    if (result){
        free(answer);
        return push_error(result);
    }
    for (int i = 0; answer[i] != '\0';i++){
        printf("%c", answer[i]);
    }
    free(answer);
    return 0;
}