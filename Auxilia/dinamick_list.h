#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VECTOR_INITIAL_CAPACITY 4

typedef struct {
    void* data;
    size_t element_size;
    size_t capacity;
    size_t size;
} Vector;


void vec_init_my(Vector* vec, size_t element_size);

void resize_my(Vector* vec, size_t new_capacity);

void push_my(Vector* vec, void* element);

void* get_my(Vector* vec, size_t index);

void pop_my(Vector* vec, void* out_element);

void free_vec_my(Vector* vec);
