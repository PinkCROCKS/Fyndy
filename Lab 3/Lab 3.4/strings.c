#include "strings.h"

int find_error(enum errors key){
    switch (key) {
        case input_error:
            printf("Mistake with input");
            return 1;
        case amount_arguments_error:
            printf("Mistake with amount of arguments");
            return 1;
        case same_files_error:
            printf("All files must be different");
            return 1;
        case open_error:
            printf("No such file or Directory");
            return 1;
        default:
            printf("Unknown Mistake");
            return 1;
    }
}

int size_of_str(const char *str) {
    int k = 0;
    while (str[k] != '\0') {
        k++;
    }
    return k;
}

int create_str(String* current, char* content){
    current->size = size_of_str(content);
    char* yu = (char *)malloc(current->size + 1);
    if (yu == NULL){
        return memory_error;
    }
    current->content = yu;
    for (int i = 0;i < current->size;i++){
        current->content[i] = content[i];
    }
    current->content[current->size] = '\0';
    return 0;
}

void clear_str(String* current){
    current->size = 0;
    free(current->content);
}

int my_strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

int compare_str(String* first, String * second){
    if (first->size != second->size){
        return first->size - second->size;
    }
    return my_strcmp(first->content, second->content);
}

int equal_str(String* first, String * second){
    if (first->size != second->size){
        return 0;
    }
    return !(my_strcmp(first->content, second->content));
}

int string_to_int(const String* s1) {
    int sum = 0;
    for (int i = 0; i < s1->size; ++i) {
        sum = sum * 10 + (s1->content[i] - '0');
    }
    return sum;
}

int copy_str(String* first, const String* second){
    if (first->size != second->size){
        first->size = second->size;
        char * yu = (char * ) realloc(first->content, first->size + 1);
        if (yu == NULL){
            return memory_error;
        }
        first->content = yu;
    }
    for (int i = 0; i < first->size; i++){
        first->content[i] = second->content[i];
    }
    first->content[first->size] = '\0';
    return 0;
}

int copy_new_str(String ** new, const String* second){
    String* temp = (String *) malloc(sizeof(String));
    if (temp == NULL){
        return memory_error;
    }
    if (create_str(temp, second->content)){
        return memory_error;
    }
    *new = temp;
    return 0;
}

int concatenate(String* str1, String* str2){
    int new_size = str1->size + str2->size;
    char* temp = (char *) realloc(str1->content, new_size + 1);
    if (temp == NULL){
        return memory_error;
    }
    str1->content = temp;
    for (int i = str1->size; i < new_size; i++){
        str1->content[i] = str2->content[i - str1->size];
    }
    str1->content[new_size] = '\0';
    return 0;
}

void print_string(FILE* stream, String* vector, char* separator) {
    for (int i = 0; i < vector->size; ++i) {
        fprintf(stream, "%c%s", vector->content[i], separator);
    }
}