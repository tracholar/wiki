#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MALLOC(type, n) (type *) malloc(sizeof(type) * n)

int buff_size = 1024;
char * buff = MALLOC(char, buff_size);

void read_libsvm(FILE *fp, )