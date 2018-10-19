#include<stdio.h>
#include<stdlib.h>

float randd(){
    return 1.0*rand() / RAND_MAX;
}
int main(){
    printf("Hello world!\n");
    FILE * fp = fopen("data.txt", "w");
    if(fp == NULL) exit(1);
    for(int i=0; i<1000; i++){
        fprintf(fp, "%d:%f\n", i, randd());
    }
    fclose(fp);
}