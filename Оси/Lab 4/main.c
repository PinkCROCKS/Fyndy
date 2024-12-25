#include <stdio.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEMORY_SIZE 1024 * 1024

typedef struct Allocator Allocator;

typedef Allocator *create_allocator_func(void *memory, size_t size);

typedef void *allocator_alloc_func(Allocator *const allocator, const size_t size);

typedef void allocator_free_func(Allocator *const allocator, void *const memory);

typedef void allocator_destroy_func(Allocator *const allocator);

static create_allocator_func *create_allocator;
static allocator_alloc_func *allocator_alloc;
static allocator_free_func *allocator_free;
static allocator_destroy_func *allocator_destroy;


int init_library(void *library) {
    create_allocator = dlsym(library, "allocator_create");
    if (create_allocator == NULL) {
        dlclose(library);
        return 1;
    }

    allocator_alloc = dlsym(library, "allocator_alloc");
    if (allocator_alloc == NULL) {
        dlclose(library);
        return 2;
    }

    allocator_free = dlsym(library, "allocator_free");
    if (allocator_free == NULL) {
        dlclose(library);
        return 3;
    }

    allocator_destroy = dlsym(library, "allocator_destroy");
    if (allocator_destroy == NULL) {
        dlclose(library);
        return 4;
    }
    return 0;
}


int main(int argc, char **argv) {
    void *library = NULL;
    if (argc == 2) {
        library = dlopen(argv[1], RTLD_LOCAL | RTLD_NOW);
    }
    if (argc != 2 || library == NULL) {
        library = dlopen("./lib2.2Zachet.so", RTLD_GLOBAL | RTLD_LAZY);
    }
    if (library == NULL) {
        return 5;
    }
    void *memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory == MAP_FAILED) {
        return 6;
    }
    int check = init_library(library);
    if(check){
        dlclose(library);
        return check;
    }
    Allocator * allocator = create_allocator(memory, MEMORY_SIZE);
    if(allocator == NULL){
        return 6;
    }
    char * test= allocator_alloc(allocator, sizeof(char) * 16);
    if(test == NULL){
        return 7;
    }
    for (int i = 0; i < 15; i++){
        test[i] = 'a' + i;
    }
    test[15] = '\0';
    write(STDOUT_FILENO, test, sizeof(test));
    allocator_free(allocator, test);
    allocator_destroy(allocator);
    dlclose(library);
    return 0;
}