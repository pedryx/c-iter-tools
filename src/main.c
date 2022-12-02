#include "iter_tools.h"

#include <stdio.h>

void map_split(char* in, iterator** out) {
    *out = split(in, "\n");
}

void map_parse(char* in, int* out) {
    *out = atoi(in);
}

void map_parse_iters(iterator** in, iterator** out) {
    *out = it_begin(*in)->map(map_parse, sizeof(int));
}

int main(void) {
    it_init();

    char* file = read_file("in.txt");
    iterator* it = split(file, "\n\n")
        ->map(map_split, sizeof(iterator*))
        ->map(map_parse_iters, sizeof(iterator*));

    while (it->move_next(it)) {
        printf("next iter:\n");
        iterator** current = it->get_current(it);
        while ((*current)->move_next(*current)) {
            int* num = (*current)->get_current(*current);
            printf("%d\n", *num);
        }
    }

    free(file);
    it_free();
    return 0;
}