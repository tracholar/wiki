/**
 * 稀疏逻辑回归
 * 
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAX_LEN 1024
#define BATCH_SIZE 32

double logloss(int * idx, double * val, int shape, double y,
    double * weight, double * dw){
    int i, j;
    double loss = 0.0, dot = 0.0, delta;
    dot = weight[0];
    for(j = 0; j < shape; j++) {
        dot += val[j] * weight[ idx[j] ]; 
    }
    loss = log(1.0 + exp(- y *  dot));
    delta = - y/(1 + exp(y * dot));

    //printf("\n%d, shape=%d, dot = %f, loss = %f, delta = %f\n", i, shape, dot, loss, delta);

    for(j = 0; j < shape; j++) dw[idx[j]] += val[j] * delta;
    dw[0] += delta; 

    return loss;
}

void apply_gradient(double * w, double *dw, int shape, double step, double lambda, double beta){
    int i;
    for(i = 0; i<shape; i++) {
        w[i] -= step * dw[i] + 0.5*lambda*w[i];
        if(w[i] < beta && w[i] > -beta) w[i] = 0; // 软阈值算法
    }
}

double rand_d(){
    return (rand() % 1000000)/1000000.0;
}
void rand_vector(double * list, int shape){
    int i;
    for(i = 0; i<shape; i++) list[i] = (rand_d() - 0.5)*2;
}

int main(){
    int * idx = (int *)malloc(sizeof(int) * MAX_LEN), shape = 0, n_samples = 10;
    double *val = (double *)malloc(sizeof(double) * MAX_LEN), y = 1, *weight = (double *)malloc(sizeof(double) * MAX_LEN),
        *dw = (double *)malloc(sizeof(double) * MAX_LEN);
    int i;
    rand_vector(weight, MAX_LEN);
    rand_vector(dw, MAX_LEN);
    
    idx[0] = 1, idx[1] = 2, idx[2] = 4, idx[3] = 5;
    val[0] = 1, val[1] = 1, val[2] = 1, val[3] = 1;
    shape = 4;
    y = 1;
    
    for(i = 0; i < 10; i++) if(weight[i] != 0.0) printf("%d:%f,", i, weight[i]);
    for(i = 0; i< 100; i++){
        printf("%d %f\n", i, logloss(idx, val, shape, y, weight, dw));
        apply_gradient(weight, dw, MAX_LEN, 0.01, 0.1, 0.001);
    }
    for(i = 0; i < 10; i++) if(weight[i] != 0.0) printf("%d:%f,", i, weight[i]);
    printf("\n");
}