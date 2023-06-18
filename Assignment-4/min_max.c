#include <limits.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 100000  // size of the array

void find_min_max(int arr[], int start_index, int end_index, int* min_val, int* max_val) {
    if (start_index == end_index) {
        *min_val = arr[start_index];
        *max_val = arr[start_index];
    } else if (end_index - start_index == 1) {
        if (arr[start_index] < arr[end_index]) {
            *min_val = arr[start_index];
            *max_val = arr[end_index];
        } else {
            *min_val = arr[end_index];
            *max_val = arr[start_index];
        }
    } else {
        int mid_index = (start_index + end_index) / 2;
        int local_min_val_left, local_max_val_left, local_min_val_right, local_max_val_right;
#pragma omp task shared(local_min_val_left, local_max_val_left)
        find_min_max(arr, start_index, mid_index, &local_min_val_left, &local_max_val_left);
#pragma omp task shared(local_min_val_right, local_max_val_right)
        find_min_max(arr, mid_index + 1, end_index, &local_min_val_right, &local_max_val_right);
#pragma omp taskwait
        {
            if (local_min_val_left < local_min_val_right) {
                *min_val = local_min_val_left;
            } else {
                *min_val = local_min_val_right;
            }
            if (local_max_val_left > local_max_val_right) {
                *max_val = local_max_val_left;
            } else {
                *max_val = local_max_val_right;
            }
        }
    }
}
void find_min_max_seq(int arr[], int start_index, int end_index, int* min_val, int* max_val) {
    if (start_index == end_index) {
        *min_val = arr[start_index];
        *max_val = arr[start_index];
    } else if (end_index - start_index == 1) {
        if (arr[start_index] < arr[end_index]) {
            *min_val = arr[start_index];
            *max_val = arr[end_index];
        } else {
            *min_val = arr[end_index];
            *max_val = arr[start_index];
        }
    } else {
        int mid_index = (start_index + end_index) / 2;
        int local_min_val_left, local_max_val_left, local_min_val_right, local_max_val_right;
        find_min_max(arr, start_index, mid_index, &local_min_val_left, &local_max_val_left);
        find_min_max(arr, mid_index + 1, end_index, &local_min_val_right, &local_max_val_right);

        *min_val = (local_min_val_left < local_min_val_right) ? local_min_val_left : local_min_val_right;
        *max_val = (local_max_val_left > local_max_val_right) ? local_max_val_left : local_max_val_right;
    }
}
int main() {
    int arr[SIZE];
    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed;

    // initialize array with random values
    for (int i = 0; i < SIZE; i++) {
        arr[i] = 1 + rand();
    }

    // find minimum and maximum using parallel recursive decomposition
    int min_val = INT_MAX;
    int max_val = INT_MIN;

    gettimeofday(&tv1, &tz);
#pragma omp parallel
    {
#pragma omp single
        {
            find_min_max(arr, 0, SIZE - 1, &min_val, &max_val);
        }
    }
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("elapsed parallel time = %f seconds.\n", elapsed);

    printf("Minimum value: %d\n", min_val);
    printf("Maximum value: %d\n", max_val);

    min_val = INT_MAX;
    max_val = INT_MIN;

    gettimeofday(&tv1, &tz);
    find_min_max_seq(arr, 0, SIZE - 1, &min_val, &max_val);
    gettimeofday(&tv2, &tz);
    elapsed = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    printf("elapsed Seq time = %f seconds.\n", elapsed);

    printf("Minimum value: %d\n", min_val);
    printf("Maximum value: %d\n", max_val);
    return 0;
}
