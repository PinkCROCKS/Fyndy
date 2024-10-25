#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

typedef enum Error_key{
    pipe_error,
    fork_error,
    wait_error,
    read_error,
    open_error,
    exec_error,
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
        case exec_error:
            text = "Mistake with exec\n";
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

int is_str_equal(char* str) {
    char* quit = "Quit";
    for (int i = 0; i < 4; i++){
        if (str[i] != quit[i]){
            return 0;
        }
    }
    return 1;
}

int main(){
    int fd[2];
    if(pipe(fd) < 0){
        return errors(pipe_error);
    }
    pid_t pid = fork();

    if (pid == -1){
        return errors(fork_error);
    }
    else if(pid == 0){
        close(fd[1]);
        //size_t size = 256;
        dup2(fd[0], STDIN_FILENO);
        if (execl("./child", "child", NULL) == -1){
            return errors(exec_error);
        }
        close(fd[0]);
        //close(file);
    } else {
        char buffer[2048];
        ssize_t k;
        ssize_t n;
        while ((k = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0){
            if (is_str_equal(buffer)){
                break;
            }
            n = write(fd[1], buffer, k);
            if (n == -1){
                return errors(write_error);
            }
        }
        n = write(fd[1], "Quit", 4);
        if (n == -1){
            return errors(write_error);
        }
        close(fd[1]);
        close(STDIN_FILENO);
        if (wait(NULL) == -1){
            return errors(wait_error);
        };
    }
    return 0;
}