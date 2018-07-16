#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#define MALLOC(type, n) (type *) malloc(sizeof(type) * n)

int w_shape = 1024000;
double * w = MALLOC(double, w_shape);

double rand_d(){
    return (rand() % 1000000)/1000000.0;
}

void * thread_main(void * args){
    int i, j;
    for(j=0; j<100; j++)
        for(i=0; i<w_shape; i++) w[i] = rand_d();
    printf("success!\n");
    pthread_exit(NULL);
}
int main(){
    int thread_number = 100;
    pthread_t *pt = MALLOC(pthread_t, thread_number);
    int i;
    for(i=0; i<thread_number; i++) pthread_create(&pt[i], NULL, thread_main, NULL);
    for(i=0; i<thread_number; i++) pthread_join(pt[i], NULL);
}