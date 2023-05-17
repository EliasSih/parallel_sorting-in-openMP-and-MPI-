#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <time.h>

#define MAX_VALUE 100

// Function to compare two elements
int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

// Function to sort array in parallel using OpenMP
void parallel_sort(int *array, int n)
{
#pragma omp parallel for
    for (int i = 0; i < n; ++i)
    {
        qsort(array + i, 1, sizeof(int), compare);
    }
}

// Function to merge two subarrays
void merge(int *array, int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            array[k] = L[i];
            i++;
        }
        else
        {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        array[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

// Function to perform merge sort on array
void merge_sort(int *array, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        merge_sort(array, left, mid);
        merge_sort(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Parse array size from command line arguments
    int array_size = atoi(argv[1]);
    int *data = malloc(array_size * sizeof(int));

    // Generate random data
    if (rank == 0)
    {
        for (int i = 0; i < array_size; ++i)
        {
            data[i] = rand() % MAX_VALUE;
        }
    }

    // Start timing
    double start_time = MPI_Wtime();

    // Distribute data among processes
    MPI_Scatter(data, array_size / size, MPI_INT, data, array_size / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process sorts its own data
    parallel_sort(data, array_size / size);

    // Gather sorted subarrays into the root process
    MPI_Gather(data, array_size / size, MPI_INT, data, array_size / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Root process merges sorted subarrays
    if (rank == 0)
    {
        merge_sort(data, 0, array_size - 1);
    }

    // End timing and print time taken
    double end_time = MPI_Wtime();
    if (rank == 0)
    {
        printf("Time taken: %f seconds\n", end_time - start_time);

        // Print the sorted array
        for (int i = 0; i < array_size; i++)
        {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
