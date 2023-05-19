#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <time.h>

#include <string.h>

#define MAX_VALUE 10000
#define MAX_THREADS 4

int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

// Helper function to find the upper bound in an array
int upper_bound(int *array, int n, int value)
{
    int l = 0, r = n;
    while (l < r)
    {
        int mid = l + (r - l) / 2;
        if (array[mid] <= value)
        {
            l = mid + 1;
        }
        else
        {
            r = mid;
        }
    }
    return l;
}

void parallel_sort(int *array, int n)
{
    int num_threads, i, *samples, *splitters;
    int segment_size, sample_distance;

    omp_set_num_threads(MAX_THREADS);
    num_threads = omp_get_max_threads();
    segment_size = n / num_threads;
    sample_distance = segment_size / num_threads;

    samples = malloc(num_threads * num_threads * sizeof(int));
    splitters = malloc((num_threads - 1) * sizeof(int));

    if (!samples || !splitters)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    // Local sort
#pragma omp parallel for
    for (i = 0; i < num_threads; ++i)
    {
        qsort(array + i * segment_size, segment_size, sizeof(int), compare);
    }

    // Regular sampling
#pragma omp parallel for
    for (i = 0; i < num_threads; ++i)
    {
        int j;
        for (j = 0; j < num_threads; ++j)
        {
            samples[i * num_threads + j] = array[i * segment_size + j * sample_distance];
        }
    }

    // Reordering at root
    qsort(samples, num_threads * num_threads, sizeof(int), compare);
    for (i = 0; i < num_threads - 1; ++i)
    {
        splitters[i] = samples[(i + 1) * num_threads];
    }

    // Globally exchange
    int **buckets = malloc(num_threads * sizeof(int *));
    int *bucket_sizes = calloc(num_threads, sizeof(int));

    if (!buckets || !bucket_sizes)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    for (i = 0; i < num_threads; ++i)
    {
        buckets[i] = calloc(n, sizeof(int));
        if (!buckets[i])
        {
            fprintf(stderr, "Failed to allocate memory\n");
            exit(1);
        }
    }

#pragma omp parallel for
    for (i = 0; i < num_threads; ++i)
    {
        int j;
        for (j = 0; j < segment_size; ++j)
        {
            int elem = array[i * segment_size + j];
            int idx = upper_bound(splitters, num_threads - 1, elem); // Find bucket for elem
            if (idx < num_threads)
            {
                buckets[idx][bucket_sizes[idx]++] = elem;
            }
        }
    }

    // Local sort again
#pragma omp parallel for
    for (i = 0; i < num_threads; ++i)
    {
        qsort(buckets[i], bucket_sizes[i], sizeof(int), compare);
    }

    // Concatenate buckets
    int offset = 0;
    for (i = 0; i < num_threads; ++i)
    {
        memcpy(array + offset, buckets[i], bucket_sizes[i] * sizeof(int));
        offset += bucket_sizes[i];
    }

    // Cleanup
    for (i = 0; i < num_threads; ++i)
    {
        free(buckets[i]);
    }

    free(buckets);
    free(bucket_sizes);
    free(splitters);
    free(samples);
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
