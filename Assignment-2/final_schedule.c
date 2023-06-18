#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 1400

int A[N][N];
int B[N][N];
int C[N][N];

int main() {
    int i, j, k;
    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed;
    omp_set_num_threads(omp_get_num_procs());
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            A[i][j] = 2;
            B[i][j] = 2;
        }
    gettimeofday(&tv1, &tz);

/////////////////// static
#pragma omp parallel for private(i, j, k) shared(A, B, C) schedule(static)
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("elapsed static time = %f seconds.\n", elapsed);

    /////////////////////////////////////// dynamic

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            A[i][j] = 2;
            B[i][j] = 2;
        }
    gettimeofday(&tv1, &tz);
#pragma omp parallel for private(i, j, k) shared(A, B, C) schedule(dynamic)
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("elapsed dynamic time = %f seconds.\n", elapsed);

    ///////////////////////////// - guided

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            A[i][j] = 2;
            B[i][j] = 2;
        }
    gettimeofday(&tv1, &tz);
#pragma omp parallel for private(i, j, k) shared(A, B, C) schedule(guided)
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("elapsed guided time = %f seconds.\n", elapsed);

    ///////////////////// - serial

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            A[i][j] = 2;
            B[i][j] = 2;
        }

    gettimeofday(&tv1, &tz);
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("elapsed serial time = %f seconds.\n", elapsed);
}