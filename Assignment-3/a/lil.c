#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>

typedef struct {
    int* indices;
    double* data;
    int nnz;
} lil_row_t;

void spmv_lil_omp(const lil_row_t* rows, const double* x, double* y, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < rows[i].nnz; j++) {
            int k = rows[i].indices[j];
            sum += rows[i].data[j] * x[k];
        }
        y[i] = sum;
    }
}

void spmv_lil_seq(const lil_row_t* rows, const double* x, double* y, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = 0.0;
        for (int j = 0; j < rows[i].nnz; j++) {
            int k = rows[i].indices[j];
            double val = rows[i].data[j];
            y[i] += val * x[k];
        }
    }
}

int main() {
    int n = 10000;
    int nnz = 50000;

    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed;

    // Generate a random sparse matrix in LIL format
    lil_row_t* rows = malloc(n * sizeof(lil_row_t));
    for (int i = 0; i < n; i++) {
        rows[i].indices = malloc(nnz * sizeof(int));
        rows[i].data = malloc(nnz * sizeof(double));
        rows[i].nnz = 0;
    }
    for (int k = 0; k < nnz; k++) {
        int i = rand() % n;
        int j = rand() % n;
        double val = (double)rand() / RAND_MAX;
        rows[i].indices[rows[i].nnz] = j;
        rows[i].data[rows[i].nnz] = val;
        rows[i].nnz++;
    }

    // Generate a random dense vector
    double* x = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        x[i] = (double)rand() / RAND_MAX;
    }

    // Compute SpMV using sequential LIL format
    double* y_seq = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        y_seq[i] = 0.0;
        for (int j = 0; j < rows[i].nnz; j++) {
            int k = rows[i].indices[j];
            double val = rows[i].data[j];
            y_seq[i] += val * x[k];
        }
    }
    gettimeofday(&tv1, &tz);
    spmv_lil_seq(rows, x, y_seq, n);
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    double elapsed_seq = (double) elapsed;

    // Compute SpMV using OpenMP LIL format
    double* y_omp = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        y_omp[i] = 0.0;
    }
    gettimeofday(&tv1, &tz);
    spmv_lil_omp(rows, x, y_omp, n);
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    double elapsed_omp = (double) elapsed;

    // Compare the results
    for (int i = 0; i < n; i++) {
        if (fabs(y_seq[i] - y_omp[i]) > 1e-5) {
            fprintf(stderr, "Error: mismatch at index %d\n", i);
            return 1;
        }
    }

    int num = 10;
    printf("first %d values : \n", num);

    for (int i = 0; i < num; i++) {
        printf("%lf ", y_seq[i]);
    }
    printf("\n");

    for (int i = 0; i < num; i++) {
        printf("%lf ", y_omp[i]);
    }
    printf("\n");

    // Print the performance results
    printf("Sequential LIL: %lf s\n", elapsed_seq);
    printf("OpenMP LIL: %lf s\n", elapsed_omp);
    printf("Speedup = %lf \n", elapsed_seq/elapsed_omp);

    return 0;
}
