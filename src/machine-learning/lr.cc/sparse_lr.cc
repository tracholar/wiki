//
// Created by zuoyuan on 18/7/13.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MALLOC(type, n) (type *) malloc(sizeof(type) * n)

typedef struct _fn {
    int idx;
    double val;
} sparse_node;

void read_libsvm_data(sparse_node * x, int & x_shape, double &y, FILE *fp){

}
double logloss(sparse_node * x, int shape, double y, double * w, double b, sparse_node * dw, double & db){
    return 0.0;
}
void apply_gradient(double * w, double &b, sparse_node * dw, int shape, double db, double lr){

}
void train(){
    sparse_node *x = MALLOC(sparse_node, 1024), *dw = MALLOC(sparse_node, 1024);
    double y;

    FILE * fp = fopen("/Users/zuoyuan/Documents/code/wiki/src/machine-learning/lr.cc/data.dat", "rb");
    if(fp == NULL) exit(1);

    int nrow = 0, x_shape;
    double * w = MALLOC(double, 1024), b, db;
    while(!feof(fp)){
        nrow += 1;

        read_libsvm_data(x, x_shape, y, fp);
        logloss(x, x_shape, y, w, b, dw, db);

        if(nrow % 128 == 0){
            apply_gradient(w, b, dw, )
        }
    }
}