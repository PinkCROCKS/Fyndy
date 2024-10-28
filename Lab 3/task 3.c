#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum errors {
    zero_eror,
    input_error,
    amount_arguments_error,
    realpath_error,
    same_files_error,
    memory_error,
    open_error
} erors;

typedef enum kOpts {
    OPT_A,
    OPT_D
} kOpts;

struct Employee{
    unsigned int id;
    char* name;
    char * surname;
    double salary;
};

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

int GetOpts(int argc, char** argv, kOpts* option, int* paths){
    if (argc != 4){
        return amount_arguments_error;
    }
    int key = 1;
    int k = 0;
    for (int i = 1; i < argc; i++){
        char* procceding_option = argv[i];
        if ((key)&&(procceding_option[0] == '/' || procceding_option[0] == '-')) {
            if (procceding_option[2] != '\0'){
                return input_error;
            }
            switch (procceding_option[1]) {
                case 'd':
                    *option = OPT_D;
                    key = 0;
                    break;
                case 'a':
                    *option = OPT_A;
                    key = 0;
                    break;
                default:
                    return input_error;
            }
        }
        else {
            paths[k] = i;
            k++;
        }
    }
    return 0;
}

int same_files(const char* path1, const char* path2) {
    char* real_path1 = (char*)malloc(1024);;
    realpath(path1, real_path1);
    if (real_path1 == NULL) {
        return realpath_error;
    }
    char* real_path2 = (char*)malloc(1024);;
    realpath(path2, real_path2);
    if (real_path2 == NULL) {
        return realpath_error;
    }
    int result = (strcmp(real_path1, real_path2) == 0);
    free(real_path1);
    free(real_path2);
    return result;
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

int my_strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

void copy_str(char* answer, const char * str){
    int size = size_of_str(str);
    for (int i = 0; i < size;i++){
        answer[i] = str[i];
    }
    answer[size] = '\0';
}

void free_array(struct Employee* array, int size){
    for (int i = 0;i < size; i++){
        free(array[i].name);
        free(array[i].surname);
    }
    free(array);
}

int put_to_Array(struct Employee** array, struct Employee new_element, int* size, int i){
    if (i >= *size){
        *size += 1;
        struct Employee* yu = (struct Employee*) realloc(*array, (*size) * sizeof(struct Employee));
        if (yu == NULL){
            free_array(*array, *size - 1);
            return memory_error;
        }
        *array = yu;
    }
    (*array)[i] = new_element;
    return 0;
}

int init_employee(struct Employee* current, int id, char* name, char* surname, double salary){
    (*current).id = id;
    char * yu = (char *) malloc(size_of_str(name));
    if (yu == NULL){
        return memory_error;
    }
    (*current).name = yu;
    copy_str((*current).name, name);
    char * ya = (char *) malloc(size_of_str(name));
    if (ya == NULL){
        free((*current).name);
        return memory_error;
    }
    (*current).surname = ya;
    copy_str((*current).surname, surname);
    (*current).salary = salary;
    return 0;
}

void print_Employ(struct Employee current){
    printf("Employe:\nID: %d\nName: %s\nSurname: %s\nSalary: %f\n", current.id,
           current.name, current.surname, current.salary);
}

int Analyze_file(FILE* file, struct Employee** base, int* size){
    int i = 0;
    int a;
    char name[100];
    char surname[100];
    double f;
    int n = fscanf(file, "%u %s %s %lf", &a, name, surname, &f);
    while (n > 0){
        if (n != 4){
            return input_error;
        }
        if (f < 0){
            return input_error;
        }
        struct Employee current;
        if (init_employee(&current, a, name, surname, f)){
            return memory_error;
        }
        int result = put_to_Array(base, current, size, i);
        if (result){
            return result;
        }
        i++;
        n = fscanf(file, "%u %s %s %lf", &a, name, surname, &f);
    }
    return 0;
}

int compare(const void * val1, const void* val2){
    struct Employee * first = (struct Employee *) val1;
    struct Employee * second = (struct Employee *) val2;
    if ((*first).salary != (*second).salary){
        return (*first).salary - (*second).salary;
    }
    int n = 0;
    if (n = my_strcmp((*first).surname, (*second).surname)){
        return n;
    }
    if (n = my_strcmp((*first).name, (*second).name)){
        return n;
    }
    return (*first).id - (*second).id;
}

int compare2(const void * val1, const void* val2){
    struct Employee * first = (struct Employee *) val2;
    struct Employee * second = (struct Employee *) val1;
    if ((*first).salary != (*second).salary){
        return (*first).salary - (*second).salary;
    }
    int n = 0;
    if (n = my_strcmp((*first).surname, (*second).surname)){
        return n;
    }
    if (n = my_strcmp((*first).name, (*second).name)){
        return n;
    }
    return (*first).id - (*second).id;
}

int HandlerOptA(struct Employee* base, int* size, FILE* file){
    qsort(base, *size, sizeof(struct Employee), compare);
    for (int i = 0; i < *size; i++){
        struct Employee current = base[i];
        fprintf(file, "%u %s %s %lf\n", current.id, current.name, current.surname, current.salary);
    }
    return 0;
}

int HandlerOptD(struct Employee* base, int* size, FILE* file){
    qsort(base, *size, sizeof(struct Employee), compare2);
    for (int i = 0; i < *size; i++){
        struct Employee current = base[i];
        fprintf(file, "%u %s %s %lf\n", current.id, current.name, current.surname, current.salary);
    }
    return 0;
}

int main(int argc, char** argv){
    kOpts flag;
    int paths[2];
    int result = GetOpts(argc, argv, &flag, paths);
    if (result){
        return find_error(result);
    }
    if (same_files(argv[paths[0]], argv[paths[1]])){
        return find_error(same_files_error);
    }
    int size = 2;
    struct Employee* base = (struct Employee*) malloc(size * sizeof(struct Employee));
    if (base == NULL) {
        return find_error(memory_error);
    }
    if (same_files(argv[paths[0]], argv[paths[1]])){
        return find_error(same_files_error);
    }
    FILE * file1 = fopen(argv[paths[0]], "r");
    if (file1 == NULL){
        return find_error(open_error);
    }
    result = Analyze_file(file1, &base, &size);
    if (result){
        return find_error(result);
    }
    FILE * file2 = fopen(argv[paths[1]], "w");
    if (file2 == NULL){
        fclose(file1);
        return find_error(open_error);
    }
    int (*handlers[2])(struct Employee*, int*, FILE*) = {
            HandlerOptA,
            HandlerOptD
    };
    handlers[flag](base, &size, file2);
    fclose(file1);
    fclose(file2);
    free_array(base, size);
    //free(base);
    return 0;
}