#include "it_array.h"

#include <stdio.h>

static bool arr_move_next(iterator* self) {
    printf("move next arr\n");
    if (self->index + 1 >= self->collection_size) {
        return false;
    }

    if (self->first) {
        self->first = false;
    }
    else {
        self->index++;
    }
    return true;
}

static void* arr_get_current(iterator* self) {
    return self->collection + self->index * self->element_size;
}

iterator* arr_to_it(void* array, size_t arr_size, size_t element_size) {
    iterator* it = it_alloc();

    it->collection = array;
    it->collection_size = arr_size;
    it->element_size = element_size;
    it->index = 0;

    it->move_next = arr_move_next;
    it->get_current = arr_get_current;

    set_current_it(it);
    return it;
}