#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef enum errors{
    zero,
    mistake_with_path,
    memory_mistake,
    incorrect_symbol
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
        case incorrect_symbol:
            printf("Inocorrect symbol in this base");
            break;
        default:
            printf("Unknown mistake");
    }
    return 1;
}

void delete_zeros(const char* input, char* output) {
    int i = 0;
    while ((input[i] == '0') && (input[i] != '\0')) {
        i++;
    }
    if (input[i] == '\0') {
        output[0] = '0';
        output[1] = '\0';
    } else {
        int j = 0;
        while (input[i] != '\0') {
            output[j] = input[i];
            i++;
            j++;
        }
        output[j] = '\0';
    }
}

int size_of_str(const char * str){
    int k = 0;
    while (str[k] != '\0'){
        k++;
    }
    return k;
}

int find_index(char symbol, char* alphabet) {
    for (int i = 0; alphabet[i] != '\0'; i++) {
        if (alphabet[i] == symbol) {
            return i;
        }
    }
}

int is_lower_case(char symbol) {
    return (symbol <= 'z') && (symbol >= 'a');
}

char upper_case(char symbol) {
    if (is_lower_case(symbol)) {
        return symbol - ' ';
    }
    return symbol;
}

int correct_symbol(char c, int base){
    char* alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return (((c <= '9' && c >= '0')||(upper_case(c)<='Z' && upper_case(c)>='A'))&&
    (find_index(upper_case(c), alphabet), alphabet));
}

int char_to_digit(char symbol) {
    char c = upper_case(symbol);
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    } else {
        fprintf(stderr, "Недопустимый символ: %c\n", c);
        exit(1);
    }
}

char digit_to_char(int digit) {
    if (digit >= 0 && digit <= 9) {
        return digit + '0';
    } else if (digit >= 10 && digit <= 35) {
        return digit - 10 + 'A';
    } else {
        return incorrect_symbol;
    }
}

int max(int a, int b){
    if (a > b){
        return a;
    } else {
        return b;
    }
}

int add_in_base(int base, char** answer, int* size_of_answer, char* new_number) {
    int len_answer = size_of_str(answer[0]);
    char summand[size_of_str(new_number)];
    delete_zeros(new_number, summand);

    int len_new = size_of_str(summand);
    int max_len = max(len_answer, len_new);
    int carry = 0;
    int i;
    if (max_len + 1 > *size_of_answer) {
        *answer = (char*)realloc(*answer, (max_len + 2) * sizeof(char));
        if (*answer == NULL) {
            return memory_mistake;
        }
        *size_of_answer = max_len + 2;
    }
    for (i = len_answer; i < max_len + 1; i++) {
        (*answer)[i] = '0';
    }
    (*answer)[max_len + 1] = '\0';
    int digit_answer;
    int digit_new;
    for (i = 0; i < max_len; i++) {
        int current = len_answer - 1 - i;
        if (i < len_answer){
            digit_answer = char_to_digit((*answer)[current]);
        } else {
            digit_answer = 0;
        }
        if (i < len_new){
            if (!(correct_symbol(summand[len_new - i - 1], base))){
                return incorrect_symbol;
            }
            digit_new = char_to_digit(summand[len_new - i - 1]);
        } else {
            digit_new = 0;
        }
        int sum = digit_answer + digit_new + carry;
        carry = sum / base;
        (*answer)[max_len - i] = digit_to_char(sum % base);
    }
    if (carry > 0) {
        (*answer)[0] = digit_to_char(carry);
        if (!(correct_symbol(summand[len_new - i - 1], base))){
            return incorrect_symbol;
        }
    } else {
        for (i = 0; i < max_len; i++) {
            (*answer)[i] = (*answer)[i + 1];
        }
        (*answer)[max_len] = '\0';
    }
    return 0;
}

int base_sum(int base, int amount, char** answer, int* size, ...){
    (*answer)[0] = '0';
    (*answer)[1] = '\0';
    if (amount == 0){
        return 0;
    }
    va_list numbers;
    va_start(numbers, size);
    char * current;
    current = va_arg(numbers, char *);
    int result;
    if (amount == 1){
        result = add_in_base(base, answer, size, current);
        va_end(numbers);
        return result;
    }
    for (int i = 0; i < amount;i++){
        result = add_in_base(base, answer, size, current);
        if (result){
            return result;
        }
        current = va_arg(numbers, char *);
    }
    va_end(numbers);
    return 0;
}
int main(){
    char* first = "000001";
    char* second = "00000aR";
    int size = 7;
    char * answer = (char *)malloc(size);
    answer[0] = '0';
    answer[1] = '\0';
    int result = base_sum(36, 0, &answer, &size);
    if (result){
        free(answer);
        return find_error(result);
    }
    printf("%s", answer);
    free(answer);
    return 0;
}