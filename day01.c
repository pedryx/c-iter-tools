#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE_WIDTH 255

int main(void) {
    char line[MAX_LINE_WIDTH];
    FILE* filePtr = fopen("in.txt", "r");
    int max1 = 0;
    int max2 = 0;
    int max3 = 0;
    int current = 0;
    bool file_end;

    do {
        file_end = !fgets(line, MAX_LINE_WIDTH, filePtr);
        if (line[0] == '\n' || file_end) {
            if (current > max1) {
                max3 = max2;
                max2 = max1;
                max1 = current;
            }
            else if (current > max2) {
                max3 = max2;
                max2 = current;
            }
            else if (current > max3) {
                max3 = current;
            }
            current = 0;
        }
        else {
            current += atoi(line);
        }
    } while (!file_end);

    printf("part1: %d\n", max1);
    printf("part1: %d\n", max1 + max2 + max3);

    fclose(filePtr);
    return 0;
}