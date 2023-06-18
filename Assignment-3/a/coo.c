#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>

void spmv_coo_omp(const int* row, const int* col, const double* val, const double* x, double* y, int n, int nnz) {
    #pragma omp parallel for
    for (int i = 0; i < nnz; i++) {
        y[row[i]] += val[i] * x[col[i]];
    }
}
void spmv_coo_seq(const int* row, const int* col, const double* val, const double* x, double* y, int n, int nnz) {
   for (int i = 0; i < nnz; i++) {
        y[row[i]] += val[i] * x[col[i]];
    }
}

int main() {
    const int n =   1000000;
    const int nnz = 1000000;

    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed;

    // sparse matrix in COO format
    int* row = malloc(nnz * sizeof(int));
    int* col = malloc(nnz * sizeof(int));
    double* val = malloc(nnz * sizeof(double));

    for (int i = 0; i < nnz; i++) {
        row[i] = rand() % n;
        col[i] = rand() % n;
        val[i] = (double)rand() / RAND_MAX;
    }


    // Generate a random dense vector
    double* x = (double*)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        x[i] = (double)rand() / RAND_MAX;
    }

    // Sequential
    double* y_seq = (double*)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        y_seq[i] = 0.0;
    }

    gettimeofday(&tv1, &tz);
    spmv_coo_seq(row, col, val, x, y_seq, n, nnz);
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    double elapsed_seq = (double)elapsed;

    // Parallel
    double* y_omp = (double*) malloc(n * sizeof(double));
    
    for (int i = 0; i < n; i++) {
        y_omp[i] = 0.0;
    }
    gettimeofday(&tv1, &tz);
    spmv_coo_omp(row, col, val, x, y_omp, n, nnz);
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    double elapsed_omp = (double)elapsed;

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
    printf("Sequential COO: %lf s\n", elapsed_seq);
    printf("OpenMP COO: %lf s\n", elapsed_omp);
    printf("Speedup = %lf \n", elapsed_seq/elapsed_omp);

    free(row);
    free(col);
    free(val);
    free(x);
    free(y_seq);
    free(y_omp);

    return 0;
}
