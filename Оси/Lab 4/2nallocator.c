#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096
#define MAX_BLOCK_SIZE (PAGE_SIZE / 2)

typedef struct Block {
    size_t block_size;
    struct Block *next_free;
    bool is_free;
} Block;

typedef struct Page {
    size_t page_size;
    struct Page *next_free;
    Block *free_blocks[32];
} Page;

typedef struct Allocator {
    void *memory;
    size_t size;
    Page *free_pages;
} Allocator;

size_t round_up_to_power_of_two(size_t size) {
    if (size == 0) return 1;
    size_t power = 1;
    while (power < size) {
        power *= 2;
    }
    return power;
}

Allocator *allocator_create(void *const memory, const size_t size) {
    if (memory == NULL || size < PAGE_SIZE) {
        return NULL;
    }
    Allocator *allocator = (Allocator *) memory;
    allocator->memory = (uint8_t *) memory + sizeof(Allocator);
    allocator->size = size - sizeof(Allocator);
    allocator->free_pages = (Page *) allocator->memory;
    size_t num_pages = allocator->size / PAGE_SIZE;
    for (size_t i = 0; i < num_pages; i++) {
        Page *page = (Page *) ((uint8_t *) allocator->memory + i * PAGE_SIZE);
        page->page_size = PAGE_SIZE;
        page->next_free = (i == num_pages - 1) ? NULL : (Page *) ((uint8_t *) allocator->memory + (i + 1) * PAGE_SIZE);
        for (int j = 0; j < 32; j++) {
            page->free_blocks[j] = NULL;
        }
    }
    return allocator;
}

void allocator_destroy(Allocator *const allocator) {
    if (allocator == NULL) {
        return;
    }
    munmap(allocator, allocator->size + sizeof(Allocator));
}

void *allocator_alloc(Allocator *const allocator, const size_t size) {
    if (allocator == NULL || size == 0 || size > MAX_BLOCK_SIZE) {
        return NULL;
    }
    size_t block_size = round_up_to_power_of_two(size + sizeof(Block));
    if (block_size > MAX_BLOCK_SIZE) {
        return NULL;
    }
    Page *page = allocator->free_pages;
    while (page != NULL) {
        int index = 0;
        while ((1 << index) < block_size) {
            index++;
        }
        Block *block = page->free_blocks[index];
        if (block != NULL) {
            page->free_blocks[index] = block->next_free;
            block->is_free = false;
            return (void *) ((uint8_t *) block + sizeof(Block));
        }
        page = page->next_free;
    }
    page = allocator->free_pages;
    if (page == NULL) {
        return NULL;
    }
    allocator->free_pages = page->next_free;
    Block *block = (Block *) ((uint8_t *) page + (PAGE_SIZE - block_size));
    block->block_size = block_size;
    block->next_free = NULL;
    block->is_free = false;
    return (void *) ((uint8_t *) block + sizeof(Block));
}

void allocator_free(Allocator *const allocator, void *const memory) {
    if (allocator == NULL || memory == NULL) {
        return;
    }
    Block *block = (Block *) ((uint8_t *) memory - sizeof(Block));
    Page *page = allocator->free_pages;
    while (page != NULL) {
        if ((uint8_t *) block >= (uint8_t *) page && (uint8_t *) block < (uint8_t *) page + PAGE_SIZE) {
            int index = 0;
            while ((1 << index) < block->block_size) {
                index++;
            }

            block->next_free = page->free_blocks[index];
            block->is_free = true;
            page->free_blocks[index] = block;
            return;
        }
        page = page->next_free;
    }
}