#ifndef IT_ARRAY_H
#define IT_ARRAY_H

#include "iterator.h"

iterator* arr_to_it(void* array, size_t arr_size, size_t element_size);

#endif