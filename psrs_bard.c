// C code to implement serial version of the psrs algorithm

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Function to sort an array using regular sampling parallel sort algorithm
void regular_sampling_parallel_sort(int *array, int size){
    if (size <=1){
        //Base case: array with 0 or 1 element is already sorted
        return;
    }
    //Select a subset of elements from the array to be used as pivots
    int *pivots = (int*)malloc(sizeof(int) * size / 2);
    for (int i = 0; i < size / 2; i++){
        pivots[i] = array[i];
    }

    //Partition the array around the pivots
    int *left_array = (int*)malloc(sizeof(int) * size / 2);
    int *right_array = (int*)malloc(sizeof(int) * size / 2);
    int left_index = 0;
    int right_index = 0;
    for (int i = 0; i < size; i++){
        if (array[i] < pivots[left_index]){
            left_array[left_index++] = array[i];
        }   else {
            right_array[right_index++] = array[i];
        }
    }

    //Sort the subarrays in serial
    regular_sampling_parallel_sort(left_array, left_index);
    regular_sampling_parallel_sort(right_array, right_index);

    //Merge the sorted subarrays to form the sorted array
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < left_index && j < right_index){
        if (left_array[i] < right_array[j]){
            array[k++] = left_array[i++];
        }   else {
            array[k++] = right_array[j++];
        }
    }

    while (i < left_index){
        array[k++] = left_array[i++];
    }

    while (j < right_index){
        array[k++] = right_array[j++];
    }
    free(pivots);
    free(left_array);
    free(right_array);
}

//Driver function
int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Usage: %s <size> <element1> <element2> ...\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    if (size <= 0 || argc != size + 2){
        printf("Invalid input\n");
        return 1;
    }

    int *array = (int*)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++){
        array[i] = atoi(argv[i + 2]);
    }

    //Print the array before sorting
    printf("Array before sorting: \n");
    for (int i = 0; i < size; i++){
       printf("%d ", array[i]);
    }
    printf("\n");

    //Measure the start time
    clock_t start = clock();

    //Sort the array using regular sampling parallel sort algorithm
    regular_sampling_parallel_sort(array, size);

    //Measure the end time
    clock_t end = clock();

    //Calculate the time taken
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    //Print the array after sorting
    printf("Array after sorting:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    //Print the time taken
    printf("Time taken: %f seconds\n", time_taken);

    free(array);

    return 0;

}


//int main(){
    //Create an array of elements
//    int array[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    //Calculate the array size
    //int size = sizeof(array) / sizeof(array[0]);

    //Print the array before sorting
//    printf("Array before sorting: \n");
    //for (int i = 0; i < size; i++){
    //   printf("%d ", array[i]);
    //}
//    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++){
//        printf("%d ", array[i]);
//    }
//    printf("\n");

    //Measure the start time
//    clock_t start = clock();

    //Sort the array using regular sampling parallel sort algorithm
    //regular_sampling_parallel_sort(array, size);
//    regular_sampling_parallel_sort(array, sizeof(array) / sizeof(array[0]));

    //Measure the end time
//    clock_t end = clock();

    //Calculate the time taken
//    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    //Print the array after sorting
//    printf("Array after sorting:\n");
//    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++){
//        printf("%d ", array[i]);
//    }
//    printf("\n");

    //Print the time taken
//    printf("Time taken: %f seconds\n", time_taken);

//    return 0;
//}