#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

typedef enum Error_key{
    pipe_error,
    fork_error,
    wait_error,
    read_error,
    open_error,
    input_error,
    write_error
} Error_key;

int size_of_str(const char * str){
    int k = 0;
    while (str[k] != '\0'){
        k++;
    }
    return k;
}

int errors(Error_key key){
    char * text;
    switch (key) {
        case pipe_error:
            text = "Mistake with open Pipe\n";
            break;
        case fork_error:
            text = "Mistake with open Pipe\n";
            break;
        case wait_error:
            text = "Mistake with wait\n";
            break;
        case read_error:
            text = "Mistake with read\n";
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
    write(STDERR_FILENO, "\n", 1);
    return key;
}

int is_str_Quit(char* str) {
    char* quit = "Quit";
    for (int i = 0; i < 4; i++){
        if (str[i] != quit[i]){
            return 0;
        }
    }
    return 1;
}

int my_atof(const char* string, double* eps) {
    int sign = 1, signs = 0, dots = 0, numbers = 0;
    double result = 0.0, fraction = 1.0;
    for (int i = 0; string[i] != '\0'; ++i) {
        if (string[i] == '-' && signs == 0) {
            signs = 1;
            sign = -1;
        }
        else if (string[i] == '.' && dots == 0) {
            dots = 1;
        }
        else if (string[i] >= '0' && string[i] <= '9' && dots == 0) {
            result *= 10;
            result += (string[i] - '0');
        }
        else if (string[i] >= '0' && string[i] <= '9' && dots == 1) {
            fraction /= 10.0;
            result += (string[i] - '0') * fraction;
        }
        else {
            return input_error;
        }
    }
    *eps = result * sign;
    return 0;
}

int correct_symbol(char c){
    return ((c <= '9' && c >= '0') || (c == '.') || (c == '-'));
}

int main(){
    ssize_t n;
    size_t size = 256;
    char name_of_file[size];
    ssize_t k = read(STDIN_FILENO, name_of_file, size - 2);
    if (k == -1){
        return errors(read_error);
    } else {
        name_of_file[k - 1] = '\0';
    }
    int file = open(name_of_file, O_WRONLY | O_CREAT);
    if (file == -1){
        return errors(open_error);
    }
    char buffer[2048];
    while ((n = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0){
        if (is_str_Quit(buffer)){
            break;
        }
        int i = -1;
        int new_number = 1;
        char current[2048];
        int k = 0;
        double sum = 0.0;
        double number = 0.0;
        int incorrect_symbol = 0;
        while (buffer[++i] != '\0'){
            if (correct_symbol(buffer[i]) && new_number){
                if (!(incorrect_symbol)){
                    current[k++] = buffer[i];
                    new_number = 0;
                    incorrect_symbol = 0;
                }
            } else if (correct_symbol(buffer[i])){
                if (!(incorrect_symbol)){
                    current[k++] = buffer[i];
                }
            } else if ((buffer[i] == ' ') && (!(new_number))){
                if (!(incorrect_symbol)) {
                    current[k] = '\0';
                    int result = my_atof(current, &number);
                    if (result){
                        close(file);
                        return errors(input_error);
                    }
                }
                sum += number;
                number = 0.0;
                k = 0;
                new_number = 1;
                incorrect_symbol = 0;
            } else if (buffer[i] == ' '){
                incorrect_symbol = 0;
                continue;
            } else if (buffer[i] == '\n'){
                if (incorrect_symbol){
                    char output[1024] = "";
                    sprintf(output, "%f\n", sum);
                    if (write(file, output, size_of_str(output)) == 0){
                        return errors(write_error);
                    }
                    sum = 0.0;
                }
                current[k] = '\0';
                int result = my_atof(current, &number);
                if (result){
                    close(file);
                    return errors(input_error);
                }
                sum += number;
                number = 0.0;
                k = 0;
                new_number = 1;
                char output[1024] = "";
                sprintf(output, "%f\n", sum);
                if (write(file, output, size_of_str(output)) == 0){
                    return errors(write_error);
                }
                sum = 0.0;
                break;
            }
            else {
                incorrect_symbol = 1;
            }
        }
    }
    if (write(STDOUT_FILENO, "Finish", 6) == 0){
        return errors(write_error);
    }
    close(file);
    return 0;
}