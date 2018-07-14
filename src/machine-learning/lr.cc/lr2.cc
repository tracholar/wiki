//
// Created by zuoyuan on 18/7/14.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MALLOC(type, n) (type *) malloc(sizeof(type) * n)

typedef struct _fn {
    int idx;
    double val;
} sparse_node;
int x_buff_size = 16, dw_buff_size = 16;


void read_libsvm_row(FILE *fp, sparse_node * &x, int & x_shape, double &y){
    fread(&y, 8, 1, fp); // read y
    if(y == 0.0) y = -1.0;
    //printf("%f ", y);
    fread(&x_shape, 4, 1, fp); // read size

    while(x_buff_size < x_shape){
        free(x);
        x = MALLOC(sparse_node, x_buff_size*2);
        x_buff_size *= 2;
    }

    int i;
    //读取数据
    for (i = 0; i < x_shape; i++) {
        fread(&x[i].idx, 4, 1, fp); //read idx
        fread(&x[i].val, 8, 1, fp); // read value
    }
}
void print_libsvm_row(sparse_node * x, int x_shape){
    int i;
    for(i=0; i<x_shape; i++){
        printf("%d:%g ", x[i].idx, x[i].val);
    }
    putchar('\n');
}


double logloss(sparse_node *x, int x_shape, double y, double *w, int w_shape, double b, sparse_node *dw, double &db){
    double loss = 0.0, z=b;
    int i = 0;

    // dot
    for(i=0; i<x_shape; i++) z += x[i].val * w[ x[i].idx ];
    loss = log(1 + exp(-y*z));
    db = -y/(1 + exp(-y*z));
    for(i=0; i<x_shape; i++) {
        dw[i].idx = x[i].idx;
        dw[i].val = x[i].val * db;
    }
    return loss;
}

int main(){
    FILE *fp = fopen("/Users/zuoyuan/Documents/code/wiki/src/machine-learning/lr.cc/data.dat", "rb");
    if (fp == NULL) {
        fprintf(stderr, "OPEN FILE FAILED!\n");
        exit(1);
    }
    sparse_node * x = MALLOC(sparse_node, x_buff_size), *dw = MALLOC(sparse_node, dw_buff_size);
    int x_shape;
    double y, *w = MALLOC(double, 1024), b, db;

    while(!feof(fp)){
        read_libsvm_row(fp, x, x_shape, y);

        printf("%g\t", y);
        print_libsvm_row(x, x_shape);

        printf("logloss = %g\n", logloss(x, x_shape, y, w, 1024, b, dw, db));

    }
}