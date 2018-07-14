/**
 * 稀疏逻辑回归
 * 
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<list>
#define MAX_LEN 1024
using namespace std;

typedef struct _fn {
    int idx;
    double val;
} sparse_node;

typedef struct _sv {
    int size;
    sparse_node * x;
} * SparseVector;

typedef struct _dv {
    int size;
    int capacity;
    double * val;
} * DenseVector;

SparseVector CreateSparseVector(){
    SparseVector sv = (SparseVector) malloc(sizeof(_sv));
    sv->size = 0;
    return sv;
}

void DeleteSparseVector(SparseVector & sv){
    free(sv);
    sv = NULL;
}


inline void SparseVectorApend(SparseVector sv, int id, double v){
    sv->x[sv->size].idx = id;
    sv->x[sv->size].val = v;
    sv->size += 1;
}


// sv3 = sv1 + sv2

void ExpandDenseVector(double * &v, int & shape){
    double * w = (double *) malloc(sizeof(double) * shape * 2);
    memcpy(w, v, sizeof(double) * shape);
    free(v);
    v = w;
    shape *= 2;
}

// v = v + lambda * sv
void SparseVectorAddDense(double * v,  SparseVector sv, double lambda){
    int i;
    for(i = 0; i<sv->size; i++) v[ sv->idx[i] ] += lambda * sv->val[i];
}

void PrintSparseVector(SparseVector sv){
    if(sv == NULL)
        return;
    int i;
    printf("{size=%d, capacity=%d, data='", sv->size, sv->capacity);
    for(i = 0; i<sv->size; i++) printf("%d:%g ", sv->idx[i], sv->val[i]);
    printf("'}\n");
}
void PrintDenseVector(double * w, int shape){
    int i;
    for(i= 0;i<shape;i++) printf("%g ", w[i]);
    printf("\n");
}


double sparse_logloss(SparseVector x, double y,
                      double * w, SparseVector dw){
    double loss = 0.0, dot = 0.0, delta;
    dot = w[0];
    for(int j=0; j<x->size; j++){
        dot += w[ x->idx[j] ] * x->val[j];
    }
    double z = - y *  dot;
    if(z > 10) loss = z;
    else loss = log(1.0 + exp(z));

    delta = - y/(1 + exp(y * dot));

    SparseVectorApend(dw, 0, delta);
    for(int j=0; j<x->size; j++){
        SparseVectorApend(dw, x->idx[j], x->val[j] * delta);
    }

    return loss;
}

void apply_sparse_gradient(double *w, SparseVector dw, double lr, double lambda, double beta){
    int i,j;

    w[0] -= dw->val[0] * lr;

    for(i=1;i<dw->size;i++){
        w[ dw->idx[i] ] -= dw->val[i] * lr + 0.5 * lambda * w[ dw->idx[i] ];
        if(w[ dw->idx[i] ] < beta && w[ dw->idx[i] ] > -beta) w[ dw->idx[i] ] = 0.0;
    }
}

double logloss(int * idx, double * val, int shape, double y,
    double * weight, double * dw, double * db){ // dw 是稀疏梯度
    int j;
    double loss = 0.0, dot = 0.0, delta;
    dot = weight[0];
    for(j = 0; j < shape; j++) {
        dot += val[j] * weight[ idx[j] ];
    }
    double z = - y *  dot;
    if(z > 10) loss = z;
    else loss = log(1.0 + exp(z));
    delta = - y/(1 + exp(y * dot));

    //printf("\nshape=%d, dot = %f, loss = %f, delta = %f\n", shape, dot, loss, delta);

    for(j = 0; j < shape; j++) dw[ j ] = val[j] * delta;
    *db = delta;

    return loss;
}

void apply_gradient(double * w, int * idx, double *dw, double db, int shape, double step, double lambda, double beta){
    int i, j;
    w[0] -= step * db;
    for(i = 0; i<shape; i++) {
        j = idx[i]; // 特征id
        w[ j ] -= step * dw[i] + 0.5*lambda*w[j];
        if(w[j] < beta && w[j] > -beta) w[j] = 0; // 软阈值算法
    }
}

double rand_d(){
    return (rand() % 1000000)/1000000.0;
}
void rand_vector(double * list, int shape){
    int i;
    for(i = 0; i<shape; i++) list[i] = (rand_d() - 0.5)*2;
}


double * expand_list(double * origin, int size, int new_size){
    if(new_size < size)
        return origin;
    double * data = (double *)malloc(sizeof(double) * new_size);
    memcpy(data, origin, sizeof(double) * size);
    memset(data + size, 0, sizeof(double) * (new_size - size));
    //size *= 2;
    free(origin);
    return data;
}

int * expand_list(int * origin, int size, int new_size){
    if(new_size < size)
        return origin;
    int * data = (int *)malloc(sizeof(int) * new_size);
    memcpy(data, origin, sizeof(int) * size);
    memset(data + size, 0, sizeof(int) * (new_size - size));
    //size *= 2;
    free(origin);
    return data;
}

int bs__data_buff_size = 1024;
void read_libsvm_data(SparseVector sv, double &y, FILE *fp){
    fread(&y, 8, 1, fp); // read y
    if(y == 0.0) y = -1.0;
    //printf("%f ", y);
    int n_feature;
    fread(&n_feature, 4, 1, fp); // read size

    int i, idx;
    double val;
    //读取数据
    for (i = 0; i < n_feature; i++) {
        fread(&idx, 4, 1, fp); //read idx
        fread(&val, 8, 1, fp); // read value
        SparseVectorApend(sv, idx, val);
        //printf("%d:%f ", idx[i], val[i]);
    }
}

void train(){

    int feature_buff_size = MAX_LEN, weight_buff_size = MAX_LEN;
    int idx=0, shape = 0, n_samples = 10, n_feature, n_weight;

    double  val, y = 1,
            *weight = (double *)malloc(sizeof(double) * weight_buff_size),
            db = 0; // 稀疏梯度
    SparseVector x = CreateSparseVector(), dw = CreateSparseVector(), dwi = CreateSparseVector();
    int i, j;
    double cum_loss = 0.0;
    rand_vector(weight, MAX_LEN);

    for(j = 0; j < 100; j++) {
        FILE *fp = fopen("/Users/zuoyuan/Documents/code/wiki/src/machine-learning/lr.cc/data.dat", "rb");
        if (fp == NULL) {
            fprintf(stderr, "OPEN FILE FAILED!\n");
            exit(1);
        }

        //for (i = 0; i < 64; i++) if (weight[i] != 0.0) printf("%d:%.3f,", i, weight[i]);
        //printf("\n");

        int nrow = 0;
        while (!feof(fp)) {
            nrow += 1;

            read_libsvm_data(x, y, fp);

            cum_loss += sparse_logloss(x, y, weight, dw);

            apply_sparse_gradient(weight, dw, 0.001, 0.001, 0.000);




/*
            if(nrow % 128 == 0) {
                apply_sparse_gradient(weight, dw, 0.1, 0.001, 0.0001);

                dw->size=0; // 归0
                printf("loss = %f\n", cum_loss);
                cum_loss = 0;
            }
            */
        }


        printf("%d, loss=%f\n", j, cum_loss);
        cum_loss = 0;

        fclose(fp);
    }
    PrintDenseVector(weight, n_weight);
}

void test1(){
    int feature_buff_size = MAX_LEN, weight_buff_size = MAX_LEN;
    int * idx = (int *)malloc(sizeof(int) * feature_buff_size), shape = 0, n_samples = 10, n_feature, n_weight;

    double *val = (double *)malloc(sizeof(double) * feature_buff_size), y = 1,
            *weight = (double *)malloc(sizeof(double) * weight_buff_size),
            db = 0,
            *dw = (double *)malloc(sizeof(double) * feature_buff_size); // 稀疏梯度

    int i, j;
    double cum_loss = 0.0;
    rand_vector(weight, MAX_LEN);
    rand_vector(dw, MAX_LEN);

    idx[0] = 1, idx[1] = 3, idx[2] = 5;
    val[0] = 1, val[1] = 1, val[2] =1;
    n_feature = 3;
    for(i = 0; i<6; i++) printf("%d:%f ", i, weight[i]);
    cum_loss = logloss(idx, val, n_feature, 1, weight, dw, &db);
    printf("%f\n", cum_loss);

    train();

}

void test_sparse_vector(){
    SparseVector  sv = CreateSparseVector();
    SparseVectorApend(sv, 1, 1);
    SparseVectorApend(sv, 1024, 1);
    PrintSparseVector(sv);

    int i;
    for(i = 0; i< 2048; i++) SparseVectorApend(sv, i, rand_d());
    PrintSparseVector(sv);

    DeleteSparseVector(sv);
    PrintSparseVector(sv);
}

void test_sparse_vector_add(){
    SparseVector sv1 = CreateSparseVector(), sv2 = CreateSparseVector();
    SparseVectorApend(sv1, 1, 1);
    SparseVectorApend(sv1, 10, 1);
    SparseVectorApend(sv1, 1024, 1);

    SparseVectorApend(sv2, 2, 1);
    SparseVectorApend(sv2, 1024, 1);

    PrintSparseVector(sv1);
    PrintSparseVector(sv2);
    PrintSparseVector(SparseVectorAdd(sv1, sv2));
}

void teset_dense_sparse_add(){
    SparseVector sv1 = CreateSparseVector();
    SparseVectorApend(sv1, 1, 1);
    SparseVectorApend(sv1, 10, 1);
    PrintSparseVector(sv1);

    double * w = new double[16];
    rand_vector(w, 16);
    int i;
    for(i=0;i<16;i++) printf("%g,", w[i]);
    printf("\n");

    SparseVectorAddDense(w, sv1, 0.5);
    for(i=0;i<16;i++) printf("%g,", w[i]);
    printf("\n");
}

void test_sparse_log_loss(){
    SparseVector dw = CreateSparseVector(), x = CreateSparseVector();
    double * w = new double[1024], y = 1.0;

    rand_vector(w, 1024);

    PrintDenseVector(w, 8);

    SparseVectorApend(x, 1, 1);
    SparseVectorApend(x, 6, 0.5);
    PrintSparseVector(x);
    printf("loss=%f\n", sparse_logloss(x, y, w, dw));
    PrintSparseVector(dw);

    apply_sparse_gradient(w, dw, 0.1, 0.0, 0.0);
    PrintDenseVector(w, 8);


}
int main(){
    test_sparse_vector();
    test_sparse_vector_add();

    printf("test teset_dense_sparse_add\n");
    teset_dense_sparse_add();

    printf("test_sparse_log_loss\n");
    test_sparse_log_loss();

    train();
}