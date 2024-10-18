#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

typedef enum errors{
    zero,
    mistake_with_path,
    memory_mistake
} errors;

int find_error(int key) {
    if (!(key)) {
        return 0;
    }
    switch (key) {
        case mistake_with_path:
            printf("No such file or directory");
            break;
        case memory_mistake:
            printf("Memory error");
            break;
        default:
            printf("Unknown mistake");
    }
    return 1;
}

int size_of_str(const char * str){
    int k = 0;
    while (str[k] != '\0'){
        k++;
    }
    return k;
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

void left_shift(char* str, int size, char new_symbol){
    for (int i = 1; i < size;i++){
        str[i - 1] = str[i];
    }
    str[size - 1] = new_symbol;
}

int find_in_File(const char* str, const char* path, char* current_str, int size, int * amount_of_strs) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return mistake_with_path;
    }
    if (current_str == NULL){
        fclose(file);
        return memory_mistake;
    }
    int number_of_str = 1;
    int number_of_symbol = 0;
    char c = getc(file);
    int len = 0;
    while (c != EOF){
        if (len < size){
            current_str[len] = c;
            len++;
            c = getc(file);
            continue;
        }
        if (is_str_equal(current_str, str)){
            *amount_of_strs = 0;
            printf("str: %d, symbol: %d\n", number_of_str, number_of_symbol);
        }
        left_shift(current_str, size, c);
        number_of_symbol++;
        if (current_str[0] == '\n'){
            number_of_str++;
            number_of_symbol = 0;
        }
        c = fgetc(file);
    }
    if (is_str_equal(current_str, str)){
        *amount_of_strs = 0;
        printf("str: %d, symbol: %d\n", number_of_str, number_of_symbol);
    }
    left_shift(current_str, size, c);
    number_of_symbol++;
    if (current_str[0] == '\n'){
        number_of_str++;
        number_of_symbol = 0;
    }
    fclose(file);
    return 0;
}


int findstr(char * str, ...){
    va_list paths;
    va_start(paths, str);
    char * k = va_arg(paths, char*);
    int result;
    int size = size_of_str(str);
    char * current_str = (char *) malloc(size + 1);
    int amount_of_strs = 1;
    while (k != NULL){
        printf("File: %s\n", k);
        result = find_in_File(str, k, current_str, size, &amount_of_strs);
        if (result == mistake_with_path){
            printf("No such file\n");
            k = va_arg(paths, char*);
            continue;
        } else if (result){
            return find_error(result);
        }
        if (amount_of_strs) {
            printf("No coincidence\n");
        }
        amount_of_strs = 1;
        k = va_arg(paths, char*);
    }
    free(current_str);
    va_end(paths);
}

int main(){
    findstr("\n", "1.txt", "3.txt", NULL);
    return 0;
}