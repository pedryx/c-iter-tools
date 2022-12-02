#include "it_array.h"

#include <stdio.h>

static bool arr_move_next(iterator* self) {
    if (self->index + 1 >= self->_collection_size) {
        return false;
    }

    if (self->_first) {
        self->_first = false;
    }
    else {
        self->index++;
    }
    return true;
}

static void* arr_get_current(iterator* self) {
    return (void*)((uintptr_t)self->_collection + self->index * self->_element_size);
}

iterator* arr_to_it(void* array, size_t arr_size, size_t element_size) {
    iterator* it = _it_alloc();

    it->_collection = array;
    it->_collection_size = arr_size;
    it->_element_size = element_size;
    it->index = 0;

    it->move_next = arr_move_next;
    it->get_current = arr_get_current;

    _set_current_it(it);
    return it;
}