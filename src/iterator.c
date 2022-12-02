#include "iterator.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE_COLLECTION_SIZE 8

/**
 * Represent iterator which is currently in use. This is used for method chaining.
*/
static iterator* current_it = NULL;

/**
 * Iterators are stored in segments of size BASE_COLLECTION_SIZE, so we can avoid invalidating pointers via
 * realloc.
*/
static iterator** it_collection;
static size_t it_segment = 0;
static size_t it_index = 0;
static size_t it_segments_capacity = BASE_COLLECTION_SIZE;

void it_init() {
    it_collection = (iterator**)malloc(sizeof(iterator*) * it_segments_capacity);
    it_collection[it_segment] = (iterator*)malloc(sizeof(iterator) * BASE_COLLECTION_SIZE);
}

void it_free() {
    for (size_t i = 0; i < it_segment; ++i) {
        for(size_t j = 0; j < BASE_COLLECTION_SIZE; ++j) {
            if (it_collection[i][j]._data != NULL) {
                free(it_collection[i][j]._data);
            }
            if (it_collection[i][j]._current_element != NULL) {
                free(it_collection[i][j]._current_element);
            }
        }
        free(it_collection[i]);
    }
    free(it_collection);
}

static bool it_move_next(iterator* self) {
    // check if last element was cached and delete it if it was
    if (self->_current_element != NULL) {
        free(self->_current_element);
        self->_current_element = NULL;
    }
    // call move next on parent
    bool result = self->_parent->move_next(self->_parent);
    self->index = self->_parent->index;

    return result;
}

static void* it_get_current(iterator* self) {
    // check if element is cached
    if (self->_current_element != NULL) {
        return self->_current_element;
    }

    // get input and output and call mapping function on them
    void* input = self->_parent->get_current(self->_parent);
    void* output = malloc(self->_element_size);
    self->_map_func(input, output);

    // store computed element into cache
    self->_current_element = output;
    return output;
}

static iterator* it_map(void (*func)(void*, void*), size_t element_size) {
    iterator* it = _it_alloc();

    it->_collection_size = _get_current_it()->_collection_size;
    it->_element_size = element_size;
    it->index = _get_current_it()->index;
    it->_map_func = func;
    it->_parent = _get_current_it();

    it->move_next = it_move_next;
    it->get_current = it_get_current;

    _set_current_it(it);
    return it;
}

static size_t it_collect(void*** arr) {
    iterator* current = _get_current_it();
    size_t element_count = 0;
    size_t capacity = BASE_COLLECTION_SIZE;

    *arr = malloc(sizeof(void*) * capacity);
    while (current->move_next(current)) {
        if (element_count >= capacity) {
            capacity *= 2;
            *arr = realloc(*arr, sizeof(void*) * capacity);
        }

        void* current_element = current->get_current(current);
        void* current_copy = malloc(current->_element_size);
        memcpy(current_copy, current_element, current->_element_size);
        (*arr)[element_count++] = current_copy;
    }

    return element_count;
}

static int32_t it_sum() {
    iterator* current = _get_current_it();
    int32_t sum = 0;

    while (current->move_next(current)) {
        sum += *(int32_t*)current->get_current(current);
    }

    return sum;
}

static size_t it_sort(void*** arr, int (*comp)(const void*, const void*)) {
    iterator* current = _get_current_it();
    size_t element_count = current->collect(arr);

    qsort(*arr, element_count, sizeof(void*), comp);

    return element_count;
}

iterator* _it_alloc() {
    // check for segment overflow
    if (it_index >= BASE_COLLECTION_SIZE) {
        // allocate new segment
        it_index = 0;
        it_segment++;
        // array with segments is full so we will expand it
        if (it_segment >= it_segments_capacity) {
            it_segments_capacity += BASE_COLLECTION_SIZE;
            it_collection = (iterator**)realloc(it_collection, sizeof(iterator*) * it_segments_capacity);
        }
        it_collection[it_segment] = (iterator*)malloc(sizeof(iterator) * BASE_COLLECTION_SIZE);
    }

    // init new iterator
    memset(&it_collection[it_segment][it_index], 0, sizeof(iterator));
    it_collection[it_segment][it_index]._first = true;
    it_collection[it_segment][it_index].map = it_map;
    it_collection[it_segment][it_index].collect = it_collect;
    it_collection[it_segment][it_index].sum = it_sum;
    it_collection[it_segment][it_index].sort = it_sort;

    return &it_collection[it_segment][it_index++];
}

void _set_current_it(iterator* it) {
    current_it = it;
}

iterator* _get_current_it() {
    return current_it;
}

iterator* it_begin(iterator* it) {
    _set_current_it(it);
    return it;
}