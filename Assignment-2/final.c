#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 3

void fill_matrix(double *matrix) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matrix[i * N + j] = (double)rand() / RAND_MAX;
}

void print_matrix(double *matrix) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%f\t", matrix[i * N + j]);
        }
        printf("\n");
    }
}

void mulitply_serial(double *a, double *b, double *c) {
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            c[i * N + j] = 0;
            for (k = 0; k < N; k++) {
                c[i * N + j] += a[i * N + k] * b[k * N + j];
            }
        }
    }
}

void multiply_matrices(double *a, double *b, double *c, int schedule) {
    int i, j, k;
    if (schedule == 1) {
#pragma omp parallel for private(i, j, k) schedule(static)
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                double sum = 0.0;
                for (k = 0; k < N; k++) {
                    sum += a[i * N + k] * b[k * N + j];
                }
                c[i * N + j] = sum;
            }
        }
    }
    if (schedule == 2) {
#pragma omp parallel for private(i, j, k) schedule(dynamic)
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                double sum = 0.0;
                for (k = 0; k < N; k++) {
                    sum += a[i * N + k] * b[k * N + j];
                }
                c[i * N + j] = sum;
            }
        }
    }
    if (schedule == 3) {
#pragma omp parallel for private(i, j, k) schedule(guided)
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                double sum = 0.0;
                for (k = 0; k < N; k++) {
                    sum += a[i * N + k] * b[k * N + j];
                }
                c[i * N + j] = sum;
            }
        }
    }
}

int main() {
    double *a = (double *)malloc(N * N * sizeof(double));
    double *b = (double *)malloc(N * N * sizeof(double));
    double *c = (double *)malloc(N * N * sizeof(double));

    fill_matrix(a);
    fill_matrix(b);

    printf("Matrix A\n");
    print_matrix(a);

    printf("Matrix B\n");
    print_matrix(b);

    printf("Using Serial code\n");
    mulitply_serial(a, b, c);
    printf("Result using serial:\n");
    print_matrix(c);

    printf("Using static schedule\n");
    multiply_matrices(a, b, c, 1);
    printf("Result using static:\n");
    print_matrix(c);

    printf("Using dynamic schedule\n");
    multiply_matrices(a, b, c, 2);
    printf("Result using dynamic:\n");
    print_matrix(c);

    printf("Using guided schedule\n");
    multiply_matrices(a, b, c, 3);
    printf("Result using guided:\n");
    print_matrix(c);

    free(a);
    free(b);
    free(c);

    return 0;
}
