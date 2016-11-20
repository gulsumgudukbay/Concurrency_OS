#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include "hash.h"
HashTable *ht1; // space allocated inside library

typedef struct PARAMETER {
    int tid;
    int nInsThreads;
    int N;
} Parameter;

void * insertThread(void * ptr) {

    Parameter * parameter = (Parameter *) ptr;

    int i = 0;

    for (i = parameter->tid; i < parameter->N; i += parameter->nInsThreads)
    {
        hash_insert(ht1, i*10000, parameter->tid);
    }

    pthread_exit(NULL);
}

void * deleteThread(void * ptr) {

    Parameter * parameter = (Parameter *) ptr;

    int i = 0;
    for (i = 2; i < parameter->N; i += parameter->nInsThreads) {
        printf("trynna delete... k= %d\n", i*10000);
        while(hash_delete(ht1, i*10000) < 0);
        printf("ok.\n");
    }

    pthread_exit(NULL);
}

int main() {
    int i;

    int N = 1000, M = 100;

    ht1 = hash_init(N, M);

    int nthreads = 5;
    int iret;

    Parameter parameter[nthreads];
    pthread_t threads[nthreads];

    //thread parameters
    for (int i = 0; i < nthreads; i++) {
        parameter[i].tid = i;
        parameter[i].nInsThreads = nthreads - 1;
        parameter[i].N = N;
    }

    for (i = 0; i < 4; i++) {
        iret = pthread_create(&threads[i], NULL, insertThread, &parameter[i]);
        if (iret) {
            fprintf(stderr, "Error - pthread_create() return code: %d\n", iret);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 4; i < 5; i++) {
        iret = pthread_create(&threads[i], NULL, deleteThread, &parameter[i]);
        if (iret) {
            fprintf(stderr, "Error - pthread_create() return code: %d\n", iret);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
    }

    int error = 0;

    //Check results
    for (i = 0; i < N; i += (nthreads - 1)) {
        int val;
        if (hash_get(ht1, i*10000, &val) == 0) {
            if (val != 0)
                error = 1;
        }
        else
            error = 1;
    }

    for (i = 1; i < N; i += (nthreads - 1)) {
        int val;
        if (hash_get(ht1, i*10000, &val) == 0) {
            if (val != 1)
                error = 1;
        }
        else
            error = 1;
    }

    for (i = 3; i < N; i += (nthreads - 1)) {
        int val;
        if (hash_get(ht1, i*10000, &val) == 0) {
            if (val != 3)
                error = 1;
        }
        else
            error = 1;
    }

    for (i = 2; i < N; i += (nthreads - 1)) {
        int val;
        if (hash_get(ht1, i*10000, &val) == 0) {
            error = 1;
        }
    }

    hash_destroy(ht1);

    switch (error) {
        case 0:printf("OK!\n");
            break;
        case 1:printf("NOT OK!\n");
    }

}
