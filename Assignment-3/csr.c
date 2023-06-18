#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>

void spmv_csr_seq(const int* row_ptr, const int* col_idx, const double* data, const double* x, double* y, int n) {
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int k = row_ptr[i]; k < row_ptr[i+1]; k++) {
            int j = col_idx[k];
            double val = data[k];
            sum += val * x[j];
        }
        y[i] = sum;
    }
}

void spmv_csr_omp(const int* row_ptr, const int* col_idx, const double* data, const double* x, double* y, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int k = row_ptr[i]; k < row_ptr[i+1]; k++) {
            int j = col_idx[k];
            double val = data[k];
            sum += val * x[j];
        }
        y[i] = sum;
    }
}

int main() {
    int n = 1000000;
    int nnz = 50000;

    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed;

    // Generate a random sparse matrix in CSR format
    int* row_ptr = malloc((n+1) * sizeof(int));
    int* col_idx = malloc(nnz * sizeof(int));
    double* data = malloc(nnz * sizeof(double));
    row_ptr[0] = 0;
    for (int i = 0; i < n; i++) {
        int nnz_i = 0;
        for (int j = 0; j < nnz/n; j++) {
            int k = rand() % n;
            double val = (double)rand() / RAND_MAX;
            col_idx[row_ptr[i] + nnz_i] = k;
            data[row_ptr[i] + nnz_i] = val;
            nnz_i++;
        }
        row_ptr[i+1] = row_ptr[i] + nnz_i;
    }

    // Generate a random dense vector
    double* x = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        x[i] = (double)rand() / RAND_MAX;
    }

    // Compute SpMV using sequential CSR format
    double* y_seq = malloc(n * sizeof(double));
    gettimeofday(&tv1, &tz);
    spmv_csr_seq(row_ptr, col_idx, data, x, y_seq, n);
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    double elapsed_seq = (double)elapsed;

    // Compute SpMV using OpenMP CSR format
    double* y_omp = malloc(n * sizeof(double));
    gettimeofday(&tv1, &tz);
    spmv_csr_omp(row_ptr, col_idx, data, x, y_omp, n);
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    double elapsed_omp = (double)elapsed;

    // Compare the results
    for (int i = 0; i < n; i++) {
        if (fabs(y_seq[i] - y_omp[i]) > 1e-5) {
            fprintf(stderr, "Error: mismatch at index %d\n", i);
            return 1;
        }
    }

    // Print the performance results
    printf("Sequential CSR: %lf s\n", elapsed_seq);
    printf("OpenMP CSR: %lf s\n", elapsed_omp);
    printf("Speedup = %lf \n", elapsed_seq/elapsed_omp);

    return 0;
}
