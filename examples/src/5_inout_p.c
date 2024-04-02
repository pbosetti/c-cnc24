#include <stdio.h>
#include <stdlib.h>

#define MAX_ARR_SIZE (20) // Num array elements

// Doxygen comment example
/**
 * @brief Function that prints int* array content
 * @param int* array which is the pointer to my array
 * @param int n which is the size of my array
 * @return void
 */
void print_array(int* array, int n) {
  // Declarations
  int i = 0; // i is pushed to the stack
  printf("The array content is: ");
  for(i=0; i<n; i++) {
    printf("%i ", array[i]);
  }
  printf("\n");
  // i,n and int* are now removed from the stack
}

int fill_array(int* array, int n) {
  // Declarations
  int i = 0; // This variable is pushed to the stack
  // Lets assign some values
  for(i=0; i<n; i++) {
    array[i] = i*2; // Assign twice the index value
  }

  return 0; // Success
  // Now i will be removed from stack
}

// Second example function
// Allocate array of integers of size n
int* f1(int n) {
  // Declarations
  int state = 0; // Success

  int* a = malloc(n * sizeof(int));
  // Here you can fill your array
  state = fill_array(a, n); // State cannot be returned

  return a;
  // state and n will be pop from the stack
}

// Memory leak example
void f_leak(int* array, int n) {
  printf("1 - f_leak(): Pointer value is %p\n", array);
  array = malloc(n * sizeof(int)); // The array pointer is changed
  printf("2 - f_leak(): Pointer value is %p\n", array);
  // Perform some operation
  fill_array(array, n);
  // I want to inspect array content
  print_array(array, n);
  // Here the array pointer is pop (deleted) (local copy)
  // !!!But the memory remains allocated causing memory leak!!!
}

// Third example
// Pass double pointer to the callee as in-out arguments
int f2(int** array, int n) {
  // Now I have a local variable that is **array
  // **array -> *array -> array[0] (first element)
  // Declarations
  int state = 0; // Success
  int i;

  // Let initialize our array
  (*array) = malloc(n * sizeof(int));

  // if((*array) == NULL) {
  //   printf("Allocation error!\n");
  // }

  // Perform some operations
  for(i=0; i<n; i++) {
    (*array)[i] = i;
  }

  return state;
}

int main(int argc, const char** argv) {
  // Declarations
  int state = 0; // Success

  // First example
  // Use malloc() in the caller and pass pointer as in-out argument
  int* array1 = malloc(MAX_ARR_SIZE * sizeof(int)); // Push to the heap memory
  state = fill_array(array1, MAX_ARR_SIZE);
  print_array(array1, MAX_ARR_SIZE);

  // If you dont need array1 anymore you have to free it!!!
  free(array1);

  // Second example
  // Call malloc() inside the function and return the pointer
  int* array2;
  array2 = f1(MAX_ARR_SIZE);
  print_array(array2, MAX_ARR_SIZE);

  // Lets free my array
  free(array2);

  // Third example
  int* array3 = NULL;
  state = f2(&array3, MAX_ARR_SIZE);
  print_array(array3, MAX_ARR_SIZE);

  // Free
  free(array3);

  // Memory leak example
  printf("\nMemory leak example\n");
  int* array_leak = NULL;
  // int array_leak[];
  printf("array_leak pointer is: %p\n", array_leak);
  // Lets try to initialize the array
  f_leak(array_leak, MAX_ARR_SIZE);
  printf("array_leak pointer is: %p\n", array_leak);

  // Cause segmentation 
  // (*array_leak) = 100000;

  // Lets remember to free the pointer
  free(array_leak);

  return 0; // Success
}