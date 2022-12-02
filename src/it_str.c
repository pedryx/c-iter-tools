#include "it_str.h"

#include <string.h>
#include <stdio.h>

struct str_data {
    char* current_pos;
    char* last_pos;
    char* delimeter;
};
typedef struct str_data str_data;

static bool str_move_next(iterator* self) {
    if (self->current_element != NULL) {
        free(self->current_element);
        self->current_element = NULL;
    }
    str_data* data = (str_data*)self->data;
    if (data->current_pos == NULL) {
        return false;
    }
    data->last_pos = data->current_pos + strlen(data->delimeter);
    if (self->first) {
        data->last_pos -= strlen(data->delimeter);
    }
    data->current_pos = strstr(data->last_pos, data->delimeter);

    if (self->first) {
        self->first = false;
    }
    else {
        self->index++;
    }

    return true;
}

static void* str_get_current(iterator* self) {
    if (self->current_element != NULL) {
        return self->current_element;
    }

    str_data* data = (str_data*)self->data;
    size_t length = (uintptr_t)data->current_pos - (uintptr_t)data->last_pos;
    if (data->current_pos == NULL) {
        length = strlen((char*)self->collection) 
            -  ((uintptr_t)data->last_pos - 1 - (uintptr_t)self->collection);
    }
    self->current_element = malloc(length + 1);
    memcpy(self->current_element, data->last_pos, length);
    ((char*)self->current_element)[length] = '\0';

    return self->current_element;
}

iterator* split(char* str, char* delimeter) {
    iterator* it = it_alloc();

    it->collection = str;
    it->index = 0;
    it->element_size = sizeof(char*);

    str_data* data = malloc(sizeof(str_data));
    data->current_pos = str;
    data->last_pos = NULL;
    data->delimeter = delimeter;
    it->data = data;

    it->move_next = str_move_next;
    it->get_current = str_get_current;

    set_current_it(it);
    return it;
}

char* read_file(char* path) {
    FILE* file_ptr = fopen(path, "r");

    fseek(file_ptr, 0, SEEK_END);
    long file_size = ftell(file_ptr) - 15;
    fseek(file_ptr, 0, SEEK_SET);

    char* buffer = malloc(file_size);
    fread(buffer, 1, file_size, file_ptr);
    fclose(file_ptr);
    buffer[file_size] = '\0';

    return buffer;
}