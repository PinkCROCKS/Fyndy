#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

const int K = 1;

pthread_mutex_t mutex;

typedef enum Error_key{
    pipe_error,
    fork_error,
    wait_error,
    read_error,
    open_error,
    exec_error,
    write_error,
    size_error,
    mutex_error,
    amount_of_arguments_error
} Error_key;

typedef struct Data{
    int i;
    int j;
    int k;
    int old_matrix[20][20];
} Data;

Data* initialize_data(int i, int j, int k, int old_matrix[k][k]) {
    Data* data = (Data*)malloc(sizeof(Data));
    data->i = i;
    data->j = j;
    data->k = k;
    for (int x = 0; x < k; x++) {
        for (int y = 0; y < k; y++) {
            data->old_matrix[x][y] = old_matrix[x][y];
        }
    }
    return data;
}

void change_data(int i, int j, int k, int old_matrix[k][k], Data* data){
    data->i = i;
    data->j = j;
    data->k = k;
    for (int x = 0; x < k; x++) {
        for (int y = 0; y < k; y++) {
            data->old_matrix[x][y] = old_matrix[x][y];
        }
    }
}

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
        case size_error:
            text = "Matrix must be with size > 3\n";
            break;
        default:
            text = "Unknown mistake\n";
            break;
    }
    write(STDERR_FILENO, text, size_of_str(text));
    write(STDERR_FILENO, "\n", 1);
    return key;
}

void generate_random_matrix(int k, int start_matrix[k][k]){
    //unsigned int n = (unsigned int) time(NULL);
    srand(time(NULL));
    for (int i = 0; i < k; i++){
        for (int j = 0; j < k;j++){
            start_matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void print_matrix(int k, int start_matrix[k][k]){
    int n;
    char print_buf[BUFSIZ];
    for (int i = 0; i < k; i++){
        for (int j = 0; j < k;j++){
            n = snprintf(print_buf, sizeof(print_buf), "%d ", start_matrix[i][j]);
            write(STDOUT_FILENO, print_buf, n);
        }
        write(STDOUT_FILENO, "\n", 1);
    }
}

int is_corner(int k, int i, int j){
    if (i == 0 && j == 0){
        return 1;
    }
    if (i == 0 && j == (k - 1)){
        return 3;
    }
    if (i == k - 1 && j == 0){
        return 2;
    }
    if (i == k - 1 && j == k - 1){
        return 4;
    }
    return 0;
}

int is_boarder(int k, int i, int j){
    if (i == 0){
        return 2;
    }
    if (j == 0){
        return 1;
    }
    if (i == (k - 1)){
        return 4;
    }
    if (j == (k - 1)){
        return 3;
    }
    return 0;
}

int compare(const void * first, const void * second){
    return *((int *) first) - *((int *) second);
}

int corner(int k, int matrix[20][20], int i, int j, int n){
    int array[4];
    switch (n) {
        case 1:
            array[0] = matrix[i][j];
            array[1] = matrix[i + 1][j];
            array[2] = matrix[i][j + 1];
            array[3] = matrix[i + 1][j + 1];
            break;
        case 2:
            array[0] = matrix[i][j];
            array[1] = matrix[i - 1][j];
            array[2] = matrix[i][j + 1];
            array[3] = matrix[i - 1][j + 1];
            break;
        case 3:
            array[0] = matrix[i][j];
            array[1] = matrix[i + 1][j];
            array[2] = matrix[i][j - 1];
            array[3] = matrix[i + 1][j - 1];
            break;
        case 4:
            array[0] = matrix[i][j];
            array[1] = matrix[i - 1][j];
            array[2] = matrix[i][j - 1];
            array[3] = matrix[i - 1][j - 1];
            break;
    }
    qsort(array, 4, sizeof(int), compare);
    return (array[1] + array[2]) / 2;
}

int boarder(int k, int matrix[20][20], int i, int j, int n){
    int array[6];
    int amount = 0;
    switch (n) {
        case 1:
            for (int m = -1; m < 2; m++){
                for (int y = 0;y < 2; y++){
                    array[amount++] = matrix[i + m][j + y];
                }
            }
            break;
        case 2:
            for (int m = 0; m < 2; m++){
                for (int y = -1;y < 2; y++){
                    array[amount++] = matrix[i + m][j + y];
                }
            }
            break;
        case 3:
            for (int m = -1; m < 2; m++){
                for (int y = -1;y < 1; y++){
                    array[amount++] = matrix[i + m][j + y];
                }
            }
            break;
        case 4:
            for (int m = -1; m < 1; m++){
                for (int y = -1;y < 2; y++){
                    array[amount++] = matrix[i + m][j + y];
                }
            }
            break;
    }
    qsort(array, 6, sizeof(int), compare);
    return (array[2] + array[3]) / 2;
}

int medium(int k, int matrix[20][20], int i, int j){
    int n = is_corner(k, i, j);
    if (n){
        return corner(k, matrix, i, j, n);
    }
    n = is_boarder(k, i, j);
    if (n){
        return boarder(k, matrix, i, j, n);
    }
    int amount = 0;
    int array[9];
    for (int o = -1; o < 2; o++){
        for (int l = -1; l < 2; ++l) {
            array[amount++] = matrix[i + o][l + j];
        }
    }
    qsort(array, 9, sizeof(int), compare);
    return array[4];
}

void* medium_filter(void * input_data){
    pthread_mutex_lock(&mutex);
    Data * data = (Data *) input_data;
    int * answer = (int *) malloc(sizeof(int) * 3);
    answer[0] = data->i;
    answer[1] = data->j;
    answer[2] = medium(data->k, data->old_matrix, data->i, data->j);
    if (data->j == data->k - 1){
        data->i += 1;
    }
    data->j = (data->j + 1) % data->k;
    pthread_mutex_unlock(&mutex);
    return (void *) answer;
}

int change_matrix(Data* data, int end_matrix[data->k][data->k], int limit){
    int size = data->k;
    pthread_t threads[limit];
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        return mutex_error;
    }

    int * changes;
    for (int i = 0; i < (size * size) / limit; i++){
        for (int j = 0; j < limit; j++){
            pthread_create(&threads[j], NULL, medium_filter, (void*)data);
        }
        for (int j = 0; j < limit; j++) {
            pthread_join(threads[j], (void *) &changes);
            end_matrix[changes[0]][changes[1]] = changes[2];
        }
    }
    for (int j = 0; j < (size * size) % limit; j++){
        pthread_create(&threads[j], NULL, medium_filter, (void*)data);
    }
    for (int j = 0; j < (size * size) % limit; j++){
        pthread_join(threads[j], (void *) &changes);
        end_matrix[changes[0]][changes[1]] = changes[2];
    }
    change_data(0, 0, size, end_matrix, data);
}

int main(int argc, char** argv) {
    if (argc != 2){
        return errors(amount_of_arguments_error);
    }
    int limit = atoi(argv[1]);
    int size;
    char buffer[BUFSIZ];
    int n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    if (n == -1) {
        return errors(read_error);
    } else {
        buffer[n] = '\0';
    }
    size = atoi(buffer);
    if (size < 2) {
        return errors(size_error);
    }
    int start_matrix[size][size];
    generate_random_matrix(size, start_matrix);
    print_matrix(size, start_matrix);
    int end_matrix[size][size];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            end_matrix[i][j] = 0;
        }
    }
    Data * data = initialize_data(0, 0, size, start_matrix);
    for (int k = 0; k < K; k++){
        change_matrix(data, end_matrix, limit);
    }
    write(STDOUT_FILENO, "\n", 1);
    print_matrix(size, end_matrix);
//    for (int i = 0; i < size; i++){
//        for (int j = 0;j < size; j++){
//            char print[BUFSIZ];
//            sprintf(print, "%d ", end_matrix[i][j]);
//            write(STDOUT_FILENO, print, size_of_str(print));
//        }
//        write(STDOUT_FILENO, "\n", 1);
//    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
