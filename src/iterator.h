#ifndef ITERATOR_H
#define ITERATOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

struct iterator {
    void* collection;
    size_t collection_size;
    size_t element_size;
    size_t index;
    bool first;
    void (*map_func)(void*, void*);
    struct iterator* parent;
    void* current_element;
    void* data;

    bool (*move_next)(struct iterator*);
    void* (*get_current)(struct iterator*);
    struct iterator* (*map)(void (*func)(void*, void*),size_t);
    size_t (*collect)(void***);
    int32_t (*sum)();
    size_t (*sort)(void***, int (*comp)(const void*, const void*));
};
typedef struct iterator iterator;

bool it_init();
void it_free();
iterator* it_alloc();

void set_current_it(iterator* it);
iterator* get_current_it();

iterator* it_begin(iterator* it);

#endif