#ifndef ITERATOR_H
#define ITERATOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

struct iterator {
    void* _collection;
    size_t _collection_size;
    size_t _element_size;
    bool _first;
    void (*_map_func)(void*, void*);
    struct iterator* _parent;
    void* _current_element;
    void* _data;

    // Index of current element.
    size_t index;

    /**
     * Move to the next element. Returns true if succes otherwise false.
     * No moving occur during first use!
    */
    bool (*move_next)(struct iterator*);
    /**
     * Get pointer to the current element. If called on first element and move_next
     * has not been called previously then this function has undefined behaviour!
    */
    void* (*get_current)(struct iterator*);
    /**
     * Maps current collection to some other collection via mapping function.
     * 
     * @param element_size Size in bytes of element in transformed collection.
    */
    struct iterator* (*map)(void (*func)(void*, void*), size_t element_size);
    /**
     * Enumerate through all the elements and store them into array. Each element
     * is stored as a pointer to that element. This function is not responsible for
     * managing memmory of allocated array and individual elements.
     * 
     * @return Size of collection.
    */
    size_t (*collect)(void*** arr);
    /**
     * Enumerate and sum all elements in collection. This function works only with
     * int32_t if used on other types then thi function has undefined behaviour.
    */
    int32_t (*sum)();
    /**
     * Calls collect and then sorts the array by qsort.
    */
    size_t (*sort)(void***, int (*comp)(const void*, const void*));
};
typedef struct iterator iterator;

/**
 * Initialize iterator utils framework.
*/
void it_init();
/**
 * Cleanup after oterator utils framework.
*/
void it_free();
/**
 * Prepare iterator for function chaining.
*/
iterator* it_begin(iterator* it);

/**
 * Allocates new iterator.
*/
iterator* _it_alloc();

void _set_current_it(iterator* it);
iterator* _get_current_it();

#endif