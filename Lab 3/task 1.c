#include <stdio.h>

int add(int a, int b){
    int carry;
    while (b != 0){
        carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

int size_of_str(const char *str) {
    int k = 0;
    while (str[k] != '\0') {
        k = add(k, 1);
    }
    return k;
}

void reverse(char* array){
    int i = 0;
    int b = add(size_of_str(array), -1);
    while (i < b){
        char t = array[i];
        array[i] = array[b];
        array[b] = t;
        i = add(i, 1);
        b = add(b, -1);
    }
}

void right_shift(char* array){
    int size = size_of_str(array);
    array[add(size, 1)] = '\0';
    for (int i = size; i > 0; i = add(i, -1)){
        array[i] = array[add(i, -1)];
    }
}

void translate_to_system(int dec, char* result, int r){
    if (dec == 0){
        result[0] = '0';
        result[1] = '\0';
        return;
    }
    int a  = dec;
    int mask = 1;
    for (int i = 0; i < r; i = add(i, 1)){
        mask = mask << 1;
    }
    mask = add(mask, -1);
    int sign = 0;
    if (dec < 0){
        a = add(~dec, 1);
        sign = 1;
    }
    char * alphbet = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    for (int i = 0; i <= 31 && a > 0; i = add(i, 1)){
        result[i] = alphbet[a & mask];
        a = a >> r;
    }
    reverse(result);
    if (sign){
        right_shift(result);
        result[0] = '-';
    }
}

int main() {
    char result[34];
    translate_to_system(-65536, result, 4);
    printf("%s", result);
    return 0;
}