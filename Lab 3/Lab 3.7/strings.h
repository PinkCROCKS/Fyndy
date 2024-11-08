#ifndef LABA_3_7_STRINGS_H
#define LABA_3_7_STRINGS_H

#include <stdio.h>
#include <stdlib.h>

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
    open_error,
    date_error,
    salary_error,
    find_error
} errors;

int size_of_str(const char *);
int find_errors(enum errors);
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
int is_str_equal(const char*, const char*);

#endif //LABA_3_7_STRINGS_H
