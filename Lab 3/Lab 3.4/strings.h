#include <stdio.h>
#include <stdlib.h>

#ifndef LABA_3_4_STRING_H
#define LABA_3_4_STRING_H

typedef struct String{
    char * content;
    int size;
} String;

typedef enum errors {
    zero_eror,
    input_error,
    amount_arguments_error,
    realpath_error,
    same_files_error,
    memory_error,
    open_error
} erors;

int size_of_str(const char *);
int find_error(enum errors);
int create_str(String*, char*);
void clear_str(String*);
int my_strcmp(const char*, const char*);
int compare_str(String* , String*);
int equal_str(String*, String*);
int copy_str(String*, const String*);
int copy_new_str(String**, const String*);
int concatenate(String*, String*);
void print_string(FILE*, String*, char*);
int string_to_int(const String *);
#endif //LABA_3_4_STRING_H