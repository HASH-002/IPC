Write an OpenMP program to demonstrate the use of OpenMP schedule clause in matrix-matrix multiplication. Analyse for all schedule clauses such as static, dynamic, and guided.
// Path: matrix.c       // Path: matrix.c
Write an OpenMP program to demonstrate the use of OpenMP reduction clause in matrix-matrix multiplication. Analyse for all reduction clauses such as +, *, -, and &.
// Path: matrix.c       // Path: matrix.c
Write an OpenMP program to demonstrate the use of OpenMP critical section in matrix-matrix multiplication.
#include <omp.c>
#include <stdio.c>
int main()
{
    int i, j, k;
    int a[3][3], b[3][3], c[3][3];
    printf("Enter the elements of matrix a");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            scanf("%d", &a[i][j]);
        }
    }
    printf("Enter the elements of matrix b");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            scanf("%d", &b[i][j]);
        }
    }
#pragma omp parallel for private(j, k) shared(a, b, c)
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            c[i][j] = 0;
            for (k = 0; k < 3; k++) {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
            }
        }
    }
    printf("The product of matrix a and b is");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d", c[i][j]);
        }
    }
}
