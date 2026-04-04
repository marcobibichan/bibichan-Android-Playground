/**
 * ex03-pointer-array.c
 * Purpose: Demonstrate relationship between pointers and arrays
 * Dependencies: stdio.h
 * Compile: gcc -o ex03 ex03-pointer-array.c
 * Run: ./ex03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    /* Array decays to pointer */
    printf("=== Array Decays to Pointer ===\n");
    int arr[5] = {1, 2, 3, 4, 5};
    
    printf("arr (decays to pointer): %p\n", (void*)arr);
    printf("&arr[0]: %p\n", (void*)&arr[0]);
    printf("They are the same!\n\n");
    
    /* Difference: sizeof */
    printf("=== sizeof Difference ===\n");
    printf("sizeof(arr): %zu (entire array)\n", sizeof(arr));
    printf("sizeof(int*): %zu (pointer only)\n\n", sizeof(int*));
    
    /* Pointer array vs Array pointer */
    printf("=== Pointer Array vs Array Pointer ===\n");
    
    /* Pointer array: array of pointers */
    int a = 1, b = 2, c = 3;
    int *ptr_arr[3] = {&a, &b, &c};  /* Array of 3 int pointers */
    
    printf("Pointer array:\n");
    for (int i = 0; i < 3; i++) {
        printf("ptr_arr[%d] = %p, *ptr_arr[%d] = %d\n", 
               i, (void*)ptr_arr[i], i, *ptr_arr[i]);
    }
    printf("\n");
    
    /* Array pointer: pointer to array */
    int (*arr_ptr)[5] = &arr;  /* Pointer to array of 5 ints */
    
    printf("Array pointer:\n");
    printf("arr_ptr points to entire array\n");
    printf("*arr_ptr = %p (same as arr)\n", (void*)*arr_ptr);
    printf("(*arr_ptr)[0] = %d\n", (*arr_ptr)[0]);
    printf("(*arr_ptr)[2] = %d\n\n", (*arr_ptr)[2]);
    
    /* String array (pointer array) */
    printf("=== String Array (Pointer Array) ===\n");
    const char *names[] = {"Alice", "Bob", "Charlie"};
    int name_count = sizeof(names) / sizeof(names[0]);
    
    for (int i = 0; i < name_count; i++) {
        printf("names[%d] = %s (at %p)\n", 
               i, names[i], (void*)names[i]);
    }
    printf("\n");
    
    /* 2D array and pointers */
    printf("=== 2D Array and Pointers ===\n");
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    
    printf("matrix[0][0] = %d\n", matrix[0][0]);
    printf("*(*(matrix + 0) + 0) = %d\n", *(*(matrix + 0) + 0));
    printf("matrix[1][2] = %d\n", matrix[1][2]);
    printf("*(*(matrix + 1) + 2) = %d\n", *(*(matrix + 1) + 2));
    
    /* Row pointer */
    int (*row_ptr)[3] = matrix;
    printf("\nUsing row pointer:\n");
    for (int i = 0; i < 2; i++) {
        printf("Row %d: ", i);
        for (int j = 0; j < 3; j++) {
            printf("%d ", row_ptr[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}
