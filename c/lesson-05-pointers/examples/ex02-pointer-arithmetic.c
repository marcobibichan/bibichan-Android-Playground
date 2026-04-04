/**
 * ex02-pointer-arithmetic.c
 * Purpose: Demonstrate pointer arithmetic operations
 * Dependencies: stdio.h, stdint.h
 * Compile: gcc -o ex02 ex02-pointer-arithmetic.c
 * Run: ./ex02
 */

#include <stdio.h>
#include <stdint.h>

int main(void) {
    int arr[] = {10, 20, 30, 40, 50};
    int *p = arr;
    size_t n = sizeof(arr) / sizeof(arr[0]);
    
    printf("=== Array Traversal with Pointer Arithmetic ===\n");
    printf("Array: ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");
    
    /* Method 1: Pointer + index */
    printf("Method 1 (p + i):\n");
    for (size_t i = 0; i < n; i++) {
        printf("*(p + %zu) = %d, address: %p\n", 
               i, *(p + i), (void*)(p + i));
    }
    printf("\n");
    
    /* Method 2: Pointer increment */
    printf("Method 2 (p++):\n");
    int *ptr = arr;
    for (size_t i = 0; i < n; i++) {
        printf("*ptr = %d, address: %p\n", *ptr, (void*)ptr);
        ptr++;
    }
    printf("\n");
    
    /* Pointer subtraction */
    printf("=== Pointer Subtraction ===\n");
    int *start = arr;
    int *end = arr + n;
    ptrdiff_t diff = end - start;
    printf("Elements between start and end: %td\n", diff);
    
    /* Pointer comparison */
    printf("\n=== Pointer Comparison ===\n");
    int *mid = arr + n / 2;
    printf("start < mid: %s\n", start < mid ? "true" : "false");
    printf("mid < end: %s\n", mid < end ? "true" : "false");
    printf("start == arr: %s\n", start == arr ? "true" : "false");
    
    /* Pointer arithmetic with different types */
    printf("\n=== Pointer Arithmetic with Different Types ===\n");
    
    char carr[] = {1, 2, 3, 4, 5};
    char *cp = carr;
    printf("char* + 1: address increases by %zu (sizeof char)\n", 
           (size_t)(cp + 1) - (size_t)cp);
    
    int *ip = arr;
    printf("int* + 1: address increases by %zu (sizeof int)\n",
           (size_t)(ip + 1) - (size_t)ip);
    
    double darr[] = {1.0, 2.0, 3.0};
    double *dp = darr;
    printf("double* + 1: address increases by %zu (sizeof double)\n",
           (size_t)(dp + 1) - (size_t)dp);
    
    return 0;
}
