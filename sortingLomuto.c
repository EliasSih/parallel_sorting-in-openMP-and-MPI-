// C code to implement quicksort lomuto version

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

// Partition the array using the last element as the pivot
int partition(long long arr[], int low, int high)
{
    // Choosing the pivot
    int pivot = arr[high];

    // Index of smaller element and indicates
    // the right position of pivot found so far
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {

        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {

            // Increment index of smaller element
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
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
    // printf("Sorted array: \n");
    // for (int i = 0; i < n; i++)
    //    printf("%lld ", arr[i]);
    printf("%f", time_taken);

    //printf("\nTime taken: %f seconds\n", time_taken);

    free(arr); //Free the allocated memory
    return 0;
}