#include "iterator.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE_IT_COLLECTION_SIZE 8

static iterator* current_it = NULL;

static iterator* it_collection;
static size_t it_count = 0;
static size_t it_capacity = BASE_IT_COLLECTION_SIZE;

bool it_init() {
    it_collection = (iterator*)malloc(sizeof(iterator) * it_capacity);
    return it_collection == NULL ? false : true;
}

void it_free() {
    for (int i = 0; i < it_count; ++i) {
        if (it_collection[i].data != NULL) {
            free(it_collection[i].data);
        }
        if (it_collection[i].current_element != NULL) {
            free(it_collection[i].current_element);
        }
    }
    free(it_collection);
}

static bool it_move_next(iterator* self) {
    printf("move next it\n");
    if (self->current_element != NULL) {
        free(self->current_element);
        self->current_element = NULL;
    }
    bool result = self->parent->move_next(self->parent);
    self->index = self->parent->index;

    return result;
}

static void* it_get_current(iterator* self) {
    if (self->current_element != NULL) {
        return self->current_element;
    }

    void* input = self->parent->get_current(self->parent);
    void* output = malloc(self->element_size);
    self->map_func(input, output);

    self->current_element = output;
    return output;
}

static iterator* it_map(void (*func)(void*, void*), size_t element_size) {
    iterator* it = it_alloc();

    it->collection_size = get_current_it()->collection_size;
    it->element_size = element_size;
    it->index = get_current_it()->index;
    it->map_func = func;
    it->parent = get_current_it();
    it->current_element = NULL;

    it->move_next = it_move_next;
    it->get_current = it_get_current;

    set_current_it(it);
    return it;
}

static size_t it_collect(void*** arr) {
    iterator* current = get_current_it();
    size_t element_count = 0;
    size_t capacity = BASE_IT_COLLECTION_SIZE;

    *arr = malloc(sizeof(void*) * capacity);
    while (current->move_next(current)) {
        if (element_count >= capacity) {
            capacity *= 2;
            *arr = realloc(*arr, capacity);
        }

        void* current_element = current->get_current(current);
        void* current_copy = malloc(current->element_size);
        memcpy(current_copy, current_element, current->element_size);
        (*arr)[element_count++] = current_copy;
    }

    return element_count;
}

static int32_t it_sum() {
    iterator* current = get_current_it();
    int32_t sum = 0;

    while (current->move_next(current)) {
        sum += *(int32_t*)current->get_current(current);
    }

    return sum;
}

static size_t it_sort(void*** arr, int (*comp)(const void*, const void*)) {
    iterator* current = get_current_it();
    size_t element_count = current->collect(arr);

    qsort(*arr, element_count, current->element_size, comp);

    return element_count;
}

iterator* it_alloc() {
    if (it_count >= it_capacity) {
        it_capacity *= 2;
        it_collection = (iterator*)realloc(
            it_collection,
            sizeof(iterator) * it_capacity
        );
    }

    it_collection[it_count].first = true;
    it_collection[it_count].map = it_map;
    it_collection[it_count].collect = it_collect;
    it_collection[it_count].sum = it_sum;
    it_collection[it_count].sort = it_sort;
    it_collection[it_count].data = NULL;
    it_collection[it_count].current_element = NULL;

    return &it_collection[it_count++];
}

void set_current_it(iterator* it) {
    current_it = it;
}

iterator* get_current_it() {
    return current_it;
}

iterator* it_begin(iterator* it) {
    set_current_it(it);
    return it;
}