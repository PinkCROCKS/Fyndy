#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>

typedef enum Error_key {
    shm_open_error,
    mmap_error,
    open_error,
    input_error,
    write_error
} Error_key;

int size_of_str(const char *str) {
    int k = 0;
    while (str[k] != '\0') {
        k++;
    }
    return k;
}

int errors(Error_key key) {
    char *text;
    switch (key) {
        case shm_open_error:
            text = "Mistake with shm_open\n";
            break;
        case mmap_error:
            text = "Mistake with mmap\n";
            break;
        case open_error:
            text = "Mistake with open\n";
            break;
        case input_error:
            text = "Mistake with input\n";
            break;
        case write_error:
            text = "Mistake with write\n";
            break;
        default:
            text = "Unknown mistake\n";
            break;
    }
    write(STDERR_FILENO, text, size_of_str(text));
    return key;
}

int is_str_Quit(char *str) {
    char *quit = "Quit";
    for (int i = 0; i < 4; i++) {
        if (str[i] != quit[i]) {
            return 0;
        }
    }
    return 1;
}

int my_atof(const char *string, double *eps) {
    int sign = 1, signs = 0, dots = 0, numbers = 0;
    double result = 0.0, fraction = 1.0;
    for (int i = 0; string[i] != '\0'; ++i) {
        if (string[i] == '-' && signs == 0) {
            signs = 1;
            sign = -1;
        } else if (string[i] == '.' && dots == 0) {
            dots = 1;
        } else if (string[i] >= '0' && string[i] <= '9' && dots == 0) {
            result *= 10;
            result += (string[i] - '0');
        } else if (string[i] >= '0' && string[i] <= '9' && dots == 1) {
            fraction /= 10.0;
            result += (string[i] - '0') * fraction;
        } else {
            return input_error;
        }
    }
    *eps = result * sign;
    return 0;
}

int correct_symbol(char c) {
    return ((c <= '9' && c >= '0') || (c == '.') || (c == '-'));
}

int is_str_equal(char *str) {
    char *quit = "Quit";
    for (int i = 0; i < 4; i++) {
        if (str[i] != quit[i]) {
            return 0;
        }
    }
    return 1;
}

void process_lines(const char *input, int file) {
    const char *line = input;
    char buffer[256];
    double sum;
    int result;

    while (*line != '\0') {
        int i = 0;
        while (*line != '\0' && *line != '\n') {
            buffer[i++] = *line++;
        }
        buffer[i] = '\0';
        if (*line == '\n') {
            line++;
        }
        if (*line == '\0'){
            break;
        }
        sum = 0.0;
        char *token = strtok(buffer, " ");
        while (token != NULL) {
            double number;
            if (!my_atof(token, &number)) {
                sum += number;
            }
            token = strtok(NULL, " ");
        }
        char output[1024] = "";
        sprintf(output, "%f\n", sum);
        write(file, output, size_of_str(output));
    }
}

int main(int argc, char **argv) {
    const char *shm_name = argv[2];
    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    const int SIZE = 4096;
    char buffer[SIZE];

    if (shm_fd == -1) {
        perror("shm_open");
        return errors(shm_open_error);
    }

    char *ptr = (char *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return errors(mmap_error);
    }

    int file = open(argv[1], O_WRONLY | O_CREAT);
    process_lines(ptr, file);
    if (file == -1){
        return errors(open_error);
    }

    if (write(file, "Finish", 6) == 0){
        return errors(write_error);
    }
    close(file);
    return 0;
}

//    ssize_t bytes_read;
//    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
//        if (is_str_equal(buffer)) {
//            break;
//        }
//
//        if (write(shm_fd, buffer, bytes_read) != bytes_read) {
//            perror("write");
//            return errors(write_error);
//        }
//    }
//    if (write(shm_fd, "Quit", 4) != 4) {
//        perror("write");
//        return errors(write_error);
//    }

//    int i = 0;
//    while (ptr[i] != '\0') {
//        int j = 0;
//        while (ptr[i] != '\0' && ptr[i] != '\n' && j < sizeof(buffer) - 1) {
//            buffer[j++] = ptr[i++];
//        }
//        buffer[j] = '\0';
//        if (is_str_equal(buffer)) {
//            break;
//        }
//        if (ptr[i] == '\n') {
//            i++;
//        }
//    }