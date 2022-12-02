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

void map_sums(iterator** in, int* out) {
    *out = it_begin((*in))->sum();
}

int compare(const void* a, const void* b) {
    int value_a = **(int**)a;
    int valie_b = **(int**)b;

    if (value_a > valie_b) return -1;
    if (value_a < valie_b) return 1;
    return 0;
}

int main(void) {
    it_init();

    char* file = read_file("in.txt");
    int** data = NULL;

    size_t size = split(file, "\n\n")
        ->map(map_split, sizeof(iterator*))
        ->map(map_parse_iters, sizeof(iterator*))
        ->map(map_sums, sizeof(int))
        ->sort(&data, compare);


    printf("part1: %d\n", *data[0]);
    printf("part2: %d\n", *data[0] + *data[1] + *data[2]);

    for (size_t i = 0; i < size; ++i) {
        free(data[i]);
    }
    free(data);
    //free(file);
    it_free();
    return 0;
}