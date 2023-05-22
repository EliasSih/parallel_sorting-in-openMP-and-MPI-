// C code to implement quicksort

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to swap two elements
void swap(long long int *a, long long int *b)
{
    long long int t = *a;
    *a = *b;
    *b = t;
}

void readFromFile(long long *arr, long long size)
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

// Partition the array using the first and last as the pivots

int partition(long long arr[], int low, int high)
{
    int pivot = arr[(low + high) / 2];
    int i = low - 1;
    int j = high + 1;

    while (1)
    {
        do
        {
            i++;
        } while (arr[i] < pivot);

        do
        {
            j--;
        } while (arr[j] > pivot);

        if (i >= j)
            return j;

        swap(&arr[i], &arr[j]);
    }
}

// The main function that implements QuickSort
// arr[] --> Array to be sorted,
// low --> Starting index,
// high --> Ending index
void quickSort(long long *arr, int low, int high)
{
    if (low < high)
    {

        // pi is partitioning index, arr[p]
        // is now at right place
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Driver code
int main(int argc, char *argv[])
{
     if (argc < 2)
    {
        printf("Usage: ./sorting <size>\n");
        return 1;
    }
    //int arr[] = {10, 7, 8, 9, 1, 5};
    int n = atoi(argv[1]);
    long long *arr = (long long *)malloc(n * sizeof(long long));
    
    
    //reading input stream from input file
    readFromFile(arr, n);

    clock_t start_time = clock(); //Start the clock

    // Function call
    quickSort(arr, 0, n - 1);

    clock_t end_time = clock(); //Stop the clock

    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    //Print the sorted array
    //printf("Sorted array: \n");
    //for (int i = 0; i < n; i++)
    //    printf("%lld ", arr[i]);

    printf("%f", time_taken);

    //printf("\nTime taken: %f seconds\n", time_taken);

    free(arr); //Free the allocated memory
    return 0;
}