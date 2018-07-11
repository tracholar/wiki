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

void train(){

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
            fread(&y, 8, 1, fp); // read y
            if(y == 0.0) y = -1.0;
            //printf("%f ", y);
            fread(&n_feature, 4, 1, fp); // read size

            // 扩大buff大小
            while (n_feature > 0 && feature_buff_size < n_feature) {
                idx = expand_list(idx, feature_buff_size, feature_buff_size * 2);
                val = expand_list(val, feature_buff_size, feature_buff_size * 2);

                dw = expand_list(dw, feature_buff_size, feature_buff_size * 2);
                feature_buff_size *= 2;
            }


            //读取数据
            for (i = 0; i < n_feature; i++) {
                fread(idx + i, 4, 1, fp); //read idx
                fread(val + i, 8, 1, fp); // read value
                //printf("%d:%f ", idx[i], val[i]);
            }


            n_weight = idx[n_feature - 1] + 1; // 当前需要的weight大小
            while (n_weight > 0 && weight_buff_size < n_weight) {
                weight = expand_list(weight, weight_buff_size, weight_buff_size * 2);

                weight_buff_size *= 2;
            }

            cum_loss += logloss(idx, val, n_feature, y, weight, dw, &db);

            if(nrow % 128 == 0) {
                apply_gradient(weight, idx, dw, db, n_feature, 0.001, 0.001, 0.01);
                memset(weight, 0, feature_buff_size * sizeof(double));
                db = 0;

                printf("loss = %f\n", cum_loss);
                cum_loss = 0;
            }
        }



        fclose(fp);
    }
}

int main(){
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