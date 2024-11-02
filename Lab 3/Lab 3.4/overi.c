#include "overi.h"

#include "overi.h"
#include "array.h"
#include "strings.h"

#include <stdarg.h>
#include <stdio.h>

int overfscanf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int i = 0, k = 0, err, read = 0;
    Array buffer;
    err = create_arr(5, &buffer);
    if (err)
        return -err;

    while (format[i]) {
        if (k) {
            for (int j = 0; j < buffer.length; ++j) {
                int ch1;
                seek_char(&stream, &ch1);
                if (ch1 == EOF) {
                    destroy(&buffer);

                    return -1;
                }
                if (buffer.val[j] != ch1 && ch1 > ' ') {
                    destroy(&buffer);
                    return input_error;
                }
            }

            char flaga[20];
            i += snread_value(format + i, flaga, 20, '%');
            char* flag = flaga + 1;

            if (is_str_equal(flag, "Ro")) {
                char roman[100];

                fscanf(stream, "%s", roman);

                int* result = va_arg(args, int*);

                unroman(roman, result);
                read++;
            } else if (is_str_equal(flag, "Zr")) {
                unsigned int* result = va_arg(args, unsigned int*);

                unzeckendorf(result, stream);
                read++;
            } else if (is_str_equal(flag, "Cv") || is_str_equal(flag, "CV")) {
                int* result = va_arg(args, int*);
                int base = va_arg(args, int);
                int ch;
                Array input;
                err = create_arr(5, &input);
                if (err) {
                    destroy(&buffer);
                    return -err;
                }

                seek_char(&stream, &ch);
                read_value(&stream, &input, ch);

                to_decimal(input, base, result);
                destroy(&input);
                read++;
            } else if (is_str_equal(flag, "S")) {
                Array* result = va_arg(args, Array*);
                int ch;

                seek_char(&stream, &ch);
                if (ch == EOF) {
                    destroy(&buffer);
                    return -1;
                }

                read_value(&stream, result, ch);
                read++;
            }
            else {
                int cur_read = vfscanf(stream, flaga, args);
                if (cur_read == EOF) {
                    destroy(&buffer);
                    return -1;
                }

                read += cur_read;

                va_arg(args, void*);
            }

            k = 0;
            reset(&buffer);
            continue;
        }

        if (format[i] != '%') {
            if (format[i] > ' ')
                append(&buffer, format[i]);
        } else {
            k = 1;
        }

        i++;
    }

    va_end(args);

    destroy(&buffer);

    return read;
}

int oversscanf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char* s = str;
    int i = 0, k = 0, err;
    Array buffer;
    err = create_arr(5, &buffer);
    if (err)
        return err;

    while (format[i]) {
        if (k) {
            for (int j = 0; j < buffer.length; ++j) {
                if (*s++ <= ' ') {  // TODO: fix that
                    j--;
                    continue;
                }
                s--;
                if (buffer.val[j] != *s++) {
                    destroy(&buffer);
                    return input_error;
                }
            }

            char flaga[20];
            i += snread_value(format + i, flaga, 20, '%');
            char* flag = flaga + 1;

            if (is_str_equal(flag, "Ro")) {
                char roman[100];

                sscanf(s, "%s", roman);

                int* result = va_arg(args, int*);

                unroman(roman, result);
            } else if (is_str_equal(flag, "Zr")) {
                unsigned int* result = va_arg(args, unsigned int*);

                unzeckendorf_str(result, &s);
            } else if (is_str_equal(flag, "Cv") || is_str_equal(flag, "CV")) {
                int* result = va_arg(args, int*);
                int base = va_arg(args, int);
                char ch;
                Array input;
                err = create_arr(5, &input);
                if (err) {
                    destroy(&buffer);
                    return err;
                }

                sseek_char(&s, &ch);
                sread_value(&s, &input, ch);

                to_decimal(input, base, result);
                destroy(&input);
            } else if (is_str_equal(flag, "S")) {
                Array* result = va_arg(args, Array*);
                char ch;

                sseek_char(&s, &ch);
                sread_value(&s, result, ch);
            }
            else {
                char filler;
                Array temp;
                err = create_arr(5, &temp);
                if (err)
                    return err;

                vsscanf(s, flaga, args);

                sseek_char(&s, &filler);
                sread_value(&s, &temp, 0);
                va_arg(args, void*);
                destroy(&temp);
            }

            k = 0;
            reset(&buffer);
            continue;
        }

        if (format[i] != '%') {
            if (format[i] > ' ')
                append(&buffer, format[i]);
        } else {
            k = 1;
        }

        i++;
    }

    va_end(args);

    destroy(&buffer);

    return 0;
}

int len(const char *str) {
    int i = 0;
    while(str[i++] != '\0');
    return i;
}

void string_concat(char *str1, const char *str2) {
    int i = 0, ln = len(str1) - 1;
    while (str2[i]) {
        str1[ln++] = str2[i++];
    }
    str1[ln] = '\0';
}

int is_str_equal(char *str1, char *str2) {
    while (*str1 && *str1 == *str2++)
        if (*str1++ == '\0') return 1;
    return !*str1 && !*str2;
}

int sseek_char(char **s, char *result) {
    while (**s != '\0') {
        char cur = *(*s)++;
        if (cur > ' ') {
            *result = cur;
            return 0;
        }
    }
    *result = -1;
    return 0;
}

int snread_value(const char *str, char *result, int n, char first) {
    int i = 0, j = 0;
    if (first)
        result[i++] = first;

    char character = str[j++];
    while (character > ' ' && i < n && character != '%') {
        result[i++] = character;
        character = str[j++];
    }
    result[i] = '\0';

    return i - 1;
}

int sread_value(char **s, Array *result, char first) {
    int err;
    if (first) {
        err = append(result, first);
        if (err)
            return err;
    }

    char character = *(*s)++;
    while (character > ' ') {
        err = append(result, character);
        if (err)
            return err;
        character = *(*s)++;
    }
    (*s)--;

    return 0;
}

int roman_value(char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

int unroman(char *str, int *result) {
    int total = 0;
    int i = 0;
    int prev = 0;

    while (str[i] != '\0') {
        int current_value = roman_value(str[i]);

        total += current_value;

        if (current_value > prev) {
            total -= prev * 2;
        }

        prev = current_value;
        i++;
    }

    *result = total;

    return 0;
}

int unzeckendorf(unsigned int *result, FILE *stream) {
    *result = 0;
    unsigned int prev = 0, cur = 0;
    unsigned int a = 0, b = 1, i = 0;

    while (1) {
        prev = cur;
        if(fscanf(stream, "%d", &cur) == 0)
            return input_error;

        if (prev == 1 && prev == cur)
            break;

        if (i % 2) {
            a += b;
            *result += a * cur;
        } else {
            b += a;
            *result += b * cur;
        }

        i++;
    }

    return 0;
}

int unzeckendorf_str(unsigned int *result, char **s) {
    *result = 0;
    unsigned int prev = 0, cur = 0;
    unsigned int a = 0, b = 1, i = 0;

    while (1) {
        prev = cur;
        int scanned = sscanf(*s, "%d", &cur);
        *s += 2;
        if(scanned == 0)
            return input_error;

        if (prev == 1 && prev == cur)
            break;

        if (i % 2) {
            a += b;
            *result += a * cur;
        } else {
            b += a;
            *result += b * cur;
        }

        i++;
    }
//    (*s)--;

    return 0;
}

int to_decimal(const Array x, unsigned char base, int *result) {
//    if (base < 2 || base > 36)
//        return throw_err(INCORRECT_ARGUMENTS);

    *result = 0;
    int pwr = 1;

    for (int i = x.length - 1; i >= 0; --i) {
        int chr = base_char_to_dec(x.val[i]);
//        if (chr >= base)
//            return throw_err(INCORRECT_ARGUMENTS);
        *result += chr * pwr;
        pwr *= base;
    }
    return 0;
}
