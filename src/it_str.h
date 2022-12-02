#ifndef IT_STR_H
#define IT_STR_H

#include "iterator.h"

/**
 * Create iterate over string which will iterate substring seperated by delimeter.
*/
iterator* split(char* str, char* delimeter);

/**
 * Reads whole file into string. This function is not responsible for managing memmory
 * for returned string.
*/
char* read_file(char* path);

#endif