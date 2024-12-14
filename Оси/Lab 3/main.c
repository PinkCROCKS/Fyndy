#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

typedef enum Error_key{
    pipe_error,
    fork_error,
    wait_error,
    read_error,
    open_error,
    exec_error,
    write_error,
    shm_open_error,
    mmap_error,
    input_error
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
        case shm_open_error:
            text = "Mistake with shm_open\n";
            break;
        case mmap_error:
            text = "Mistake with mmap\n";
            break;
        case input_error:
            text = "Mistake with input\n";
            break;
        default:
            text = "Unknown mistake\n";
            break;
    }
    write(STDERR_FILENO, text, size_of_str(text));
    return key;
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

int main() {
    const char *shm_name = "/my_shared_memory";
    const int SIZE = 4096; // Размер разделяемой памяти

    pid_t pid = fork();
    if (pid == -1) {
        return errors(fork_error);
    } else if (pid == 0) {
        if (execl("./child", "child", "123.txt", shm_name, NULL) == -1) {
            return errors(exec_error);
        }
    } else {
        if (wait(NULL) == -1) {
            return errors(wait_error);
        };
        int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) {
            perror("shm_open");
            return errors(shm_open_error);
        }
        if (ftruncate(shm_fd, SIZE) == -1) {
            perror("ftruncate");
            return errors(shm_open_error);
        }
        ssize_t bytes_read;
        char *ptr = (char *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED) {
            perror("mmap");
            return errors(mmap_error);
        }
        char buffer[256];
        int i = 0;
        while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(ptr))) > 0) {
            if (is_str_equal(buffer)) {
                break;
            }
            for (int j = 0; j < bytes_read; j++){
                ptr[i++] = buffer[j];
            }
        }
        for (int j = 0; j < 4; j++){
            char quit[] = "Quit";
            ptr[i++] = quit[j];
        }
        if (shm_unlink(shm_name) == -1) {
            perror("shm_unlink");
            return errors(shm_open_error);
        }
    }
    return 0;
}