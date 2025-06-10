#include "dinamick_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vec_init_my(Vector* vec, size_t element_size) {
    vec->element_size = element_size;
    vec->capacity = VECTOR_INITIAL_CAPACITY;
    vec->size = 0;
    vec->data = malloc(vec->capacity * vec->element_size);
}

void resize_my(Vector* vec, size_t new_capacity) {
    vec->data = realloc(vec->data, vec->element_size * new_capacity);
    vec->capacity = new_capacity;
}


void push_my(Vector* vec, void* element) {
    if (vec->size == vec->capacity) {
        resize_my(vec, vec->capacity * 2);
    }
    void* target = (char*)vec->data + vec->element_size * vec->size;
    memcpy(target, element, vec->element_size);
    vec->size++;
}

void* get_my(Vector* vec, size_t index) {
    if (index >= vec->size) {
        return NULL; // Or handle error
    }
    return (char*)vec->data + vec->element_size * index;
}

void pop_my(Vector* vec, void* out_element) {
    if (vec->size == 0) return;
    vec->size--;
    if (out_element) {
        void* source = (char*)vec->data + vec->element_size * vec->size;
        memcpy(out_element, source, vec->element_size);
    }
}

void free_vec_my(Vector* vec) {
    free(vec->data);
    vec->data = NULL;
    vec->size = vec->capacity = 0;
    vec->element_size = 0;
}
