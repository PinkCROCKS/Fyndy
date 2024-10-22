#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

typedef enum errors {
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

int size_of_str(const char *str) {
    int k = 0;
    while (str[k] != '\0') {
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

void left_shift(char* str, int size, char new_symbol) {
    for (int i = 1; i < size; i++) {
        str[i - 1] = str[i];
    }
    str[size - 1] = new_symbol;
}

int find_in_File(const char* str, const char* path, char* current_str, int size, int *amount_of_strs, char ***results, int *result_count) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return mistake_with_path;
    }
    if (current_str == NULL) {
        fclose(file);
        return memory_mistake;
    }
    int number_of_str = 1;
    int number_of_symbol = 1;
    char c = getc(file);
    int len = 0;
    while (c != EOF) {
        if (len < size) {
            current_str[len] = c;
            len++;
            c = getc(file);
            continue;
        }
        if (is_str_equal(current_str, str)) {
            *amount_of_strs = 0;
            char buffer[100];
            snprintf(buffer, sizeof(buffer), "File: %s, str: %d, symbol: %d\n", path, number_of_str, number_of_symbol);
            (*result_count)++;
            *results = (char **)realloc(*results, (*result_count) * sizeof(char *));
            (*results)[(*result_count) - 1] = strdup(buffer);
        }
        left_shift(current_str, size, c);
        number_of_symbol++;
        if (current_str[0] == '\n') {
            number_of_str++;
            number_of_symbol = 0;
        }
        c = fgetc(file);
    }
    if (is_str_equal(current_str, str)) {
        *amount_of_strs = 0;
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "File: %s, str: %d, symbol: %d\n", path, number_of_str, number_of_symbol);
        (*result_count)++;
        *results = (char **)realloc(*results, (*result_count) * sizeof(char *));
        (*results)[(*result_count) - 1] = strdup(buffer);
    }
    left_shift(current_str, size, c);
    number_of_symbol++;
    if (current_str[0] == '\n') {
        number_of_str++;
        number_of_symbol = 0;
    }
    fclose(file);
    return 0;
}

int findstr(char *str, char ***results, int *result_count, ...) {
    va_list paths;
    va_start(paths, result_count);
    char *k = va_arg(paths, char*);
    int result;
    int size = size_of_str(str);
    char *current_str = (char *)malloc(size + 1);
    int amount_of_strs = 1;
    int initial_result_count = *result_count;

    while (k != NULL) {
        printf("File: %s\n", k);
        result = find_in_File(str, k, current_str, size, &amount_of_strs, results, result_count);
        if (result == mistake_with_path) {
            printf("No such file\n");
            k = va_arg(paths, char*);
            continue;
        } else if (result) {
            free(current_str);
            va_end(paths);
            return find_error(result);
        }
//        if (*result_count == initial_result_count) {
//            printf("No such file\n");
//        }
        initial_result_count = *result_count;
        amount_of_strs = 1;
        k = va_arg(paths, char*);
    }
    free(current_str);
    va_end(paths);
    return 0;
}

int main() {
    char **results = NULL;
    int result_count = 0;
    printf("Files:\n");
    findstr("111", &results, &result_count, "1.txt", "2.txt", NULL);
    printf("Find conditios:\n");
    for (int i = 0; i < result_count; i++) {
        printf("%s", results[i]);
        free(results[i]);
    }
    free(results);
    return 0;
}