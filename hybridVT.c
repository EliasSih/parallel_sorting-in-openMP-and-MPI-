#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <time.h>
#include <string.h>

#define MAX_THREADS 4 

void readFromFile(int *arr, int size)
{
    FILE *file = fopen("input.txt", "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    int i = 0;
    long long num;
    char delimiter;
    while (fscanf(file, "%lld%c", &num, &delimiter) == 2 && i < size)
    {
        arr[i] = num;
        i++;
    }

    fclose(file);
}

int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

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

void parallel_sort(int *array, int n, int max_threads)
{
    // printf("%d threads\n", max_threads);
    int num_threads, i, *samples, *splitters;
    int segment_size, sample_distance;

    omp_set_num_threads(max_threads);
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

#pragma omp parallel for
    for (i = 0; i < num_threads; ++i)
    {
        qsort(array + i * segment_size, segment_size, sizeof(int), compare);
    }

#pragma omp parallel for
    for (i = 0; i < num_threads; ++i)
    {
        int j;
        for (j = 0; j < num_threads; ++j)
        {
            samples[i * num_threads + j] = array[i * segment_size + j * sample_distance];
        }
    }

    qsort(samples, num_threads * num_threads, sizeof(int), compare);
    for (i = 0; i < num_threads - 1; ++i)
    {
        splitters[i] = samples[(i + 1) * num_threads];
    }

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
            int idx = upper_bound(splitters, num_threads - 1, elem);
#pragma omp critical
            {
                buckets[idx][bucket_sizes[idx]++] = elem;
            }
        }
    }

#pragma omp parallel for
    for (i = 0; i < num_threads; ++i)
    {
        qsort(buckets[i], bucket_sizes[i], sizeof(int), compare);
    }

    int offset = 0;
    for (i = 0; i < num_threads; ++i)
    {
        memcpy(array + offset, buckets[i], bucket_sizes[i] * sizeof(int));
        offset += bucket_sizes[i];
    }

    for (i = 0; i < num_threads; ++i)
    {
        free(buckets[i]);
    }

    free(buckets);
    free(bucket_sizes);
    free(splitters);
    free(samples);
}

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

    int array_size = atoi(argv[1]);
    int *data = malloc(array_size * sizeof(int));

    readFromFile(data, array_size);

    double start_time = MPI_Wtime();

    MPI_Scatter(data, array_size / size, MPI_INT, data, array_size / size, MPI_INT, 0, MPI_COMM_WORLD);

    parallel_sort(data, array_size / size, atoi(argv[2]));

    MPI_Gather(data, array_size / size, MPI_INT, data, array_size / size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        merge_sort(data, 0, array_size - 1);
        // print time taken 
        printf("%f",MPI_Wtime() - start_time);

        // Print the sorted array
        // printf("Sorted array:\n");
        // for (int i = 0; i < array_size; i++)
        // {
        //     printf("%d ", data[i]);
        // }
        // printf("\n");
    }

    free(data);

    MPI_Finalize();
    return 0;
}
