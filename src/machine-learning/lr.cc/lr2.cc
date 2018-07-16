//
// Created by zuoyuan on 18/7/14.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#define MALLOC(type, n) (type *) malloc(sizeof(type) * n)

typedef struct _fn {
    int idx;
    double val;
} sparse_node;
int x_buff_size = 16, dw_buff_size = 16;

double rand_d(){
    return (rand() % 1000000)/1000000.0;
}
void rand_vector(double * list, int shape){
    int i;
    for(i = 0; i<shape; i++) list[i] = (rand_d() - 0.5)*2;
}

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

int w_shape;
double * w, *b;
pthread_mutex_t w_mutex = PTHREAD_MUTEX_INITIALIZER;

double logloss(sparse_node *x, int x_shape, double y, sparse_node *dw, double &db){
    double loss = 0.0, z = *b;
    int i = 0;

    // dot
    for(i=0; i<x_shape; i++) z += x[i].val * w[ x[i].idx ];
    loss = log(1 + exp(-y*z));

    if(w != NULL) {
        db = -y * exp(-y * z) / (1 + exp(-y * z));
        for (i = 0; i < x_shape; i++) {
            dw[i].idx = x[i].idx;
            dw[i].val = x[i].val * db;
        }
    }
    return loss;
}





void apply_gradient(sparse_node *dw, int dw_shape, double db, double lr, double alpha, double beta){
    int i;

    *b -= lr * db;
    for(i=0; i<dw_shape; i++){
        w[ dw[i].idx ] -= lr * dw[i].val + 0.5*alpha*w[ dw[i].idx ];
        if(fabs(w[ dw[i].idx ]) < beta) w[ dw[i].idx ] = 0;
    }

}

typedef struct _problem_params {
    char * input;
    char * output;
    double lr, lambda, beta;
    int iter, batch_size;
} problem_params;

typedef struct _thread_params {
    int tid;
    problem_params p;
}thread_params;

void * train_thread(void *tid){
    thread_params p = * (thread_params *)tid;
    printf("start thread %d\n", p.tid);

    FILE *fp = fopen(p.p.input, "rb");
    if (fp == NULL) {
        fprintf(stderr, "OPEN FILE FAILED!\n");
        exit(1);
    }
    sparse_node * x = MALLOC(sparse_node, x_buff_size), *dw = MALLOC(sparse_node, dw_buff_size);
    int x_shape;
    double y, db;

    double cumloss = 0.0;
    int nrow = 0;
    int iter, i, j;

    for(iter=0; iter<p.p.iter; iter++) {
        while (!feof(fp)) {
            nrow++;
            read_libsvm_row(fp, x, x_shape, y);


            while (dw_buff_size < x_shape) {
                free(dw);
                dw = MALLOC(sparse_node, dw_buff_size * 2);
                dw_buff_size *= 2;
            }

            cumloss += logloss(x, x_shape, y, dw, db);
            for(i=0; i<x_shape; i++){
                if(dw == NULL || dw[i].idx < 0 || dw[i].idx >= w_shape){
                    fprintf(stderr, "%d:%g\n", dw[i].idx, dw[i].val);
                }
            }

            apply_gradient(dw, x_shape, db, p.p.lr, p.p.lambda, p.p.beta);

            if (nrow % p.p.batch_size == 0) {
                printf("@%d logloss = %g\n", p.tid, cumloss / p.p.batch_size);
                cumloss = 0;
            }

        }
        fseek(fp, 0, SEEK_SET);
        printf("Thread %d finished %dth iteration.\n", p.tid, iter);
    }

    fclose(fp);
    free(dw);
    free(x);

    pthread_exit(NULL);
}

void init_problem_size(char * str){
    sparse_node * x = MALLOC(sparse_node, x_buff_size);
    double y;

    FILE *fp = fopen(str, "rb");
    if (fp == NULL) {
        fprintf(stderr, "OPEN FILE FAILED!\n");
        exit(1);
    }

    int size = 1, x_shape;
    while(!feof(fp)){
        read_libsvm_row(fp, x, x_shape, y);
        if(x[x_shape-1].idx > size - 1) size = x[x_shape-1].idx + 1;
    }
    fclose(fp);
    free(x);
    w_shape = size;
    dw_buff_size = x_buff_size;
}




int train(problem_params pp){
    init_problem_size(pp.input);
    printf("weight size=%d\n", w_shape);

    w = MALLOC(double, w_shape);
    rand_vector(w, w_shape);
    b = MALLOC(double, 1);
    *b = rand_d();

    int i, thread_number = 16;
    pthread_t *pt = MALLOC(pthread_t, thread_number);
    thread_params *p = MALLOC(thread_params, thread_number);
    for(i=0; i<thread_number; i++) {
        p[i].tid = i;
        p[i].p = pp;

        pthread_create(&pt[i], NULL, train_thread, (void *) &p[i]);
    }
    for(i=0; i<thread_number; i++) pthread_join(pt[i], NULL);
    free(pt);
    free(p);

    FILE *fp = fopen("w.txt", "w");
    fprintf(fp, "b\t%g\n", *b);
    fprintf(fp, "w\n-----------------\n");
    for(i=0; i<w_shape; i++){
        fprintf(fp, "%d\t%g\n", i, w[i]);
    }
    fclose(fp);
}

problem_params create_default_params(){
    problem_params p;
    p.input = NULL;
    p.output = "w.txt";
    p.iter = 1;
    p.lr = 0.01;
    p.batch_size = 102400;
    p.lambda = 0.001;
    p.beta = 0;
    return p;
}

int main(int argc, char **argv){
    problem_params params = create_default_params();
    params.input = "/Users/zuoyuan/Documents/code/wiki/src/machine-learning/lr.cc/data.dat";
    int i = 0;
    while(i < argc){
        if(strcmp(argv[i], "-h") == 0){
            printf("usage [OPTION]\n");
            printf("-i input\n");
            exit(0);
        }
        else if(strcmp(argv[i], "-i") == 0){
            params.input = argv[i+1];
            i += 2;
        }
        else if(strcmp(argv[i], "-o") == 0){
            params.output = argv[i+1];
            i += 2;
        }
        else if(strcmp(argv[i], "-iter") == 0){
            params.iter = atoi(argv[i+1]);
            i += 2;
        }
        else if(strcmp(argv[i], "-lr") == 0){
            sscanf(argv[i+1], "%lf", &params.lr);
            i += 2;
        }else{
            i++;
        }
    }
    train(params);
}