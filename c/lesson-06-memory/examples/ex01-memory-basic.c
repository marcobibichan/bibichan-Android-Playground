/*
 * ex01-memory-basic.c
 * Purpose: Demonstrate basic memory allocation and deallocation
 * Dependencies: stdlib.h, stdio.h
 * Compile: gcc -o ex01 ex01-memory-basic.c
 * Run: ./ex01
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    /* Declare a pointer, initialized to NULL for safety */
    int *numbers = NULL;
    
    /* Allocate memory for 10 integers
     * sizeof(int) ensures correct size on all platforms
     * malloc returns void*, which we cast to int*
     */
    numbers = (int *)malloc(10 * sizeof(int));
    
    /* ALWAYS check malloc return value
     * malloc returns NULL if allocation fails
     * Dereferencing NULL causes undefined behavior
     */
    if (numbers == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;  /* Return error code */
    }
    
    /* Initialize the allocated memory
     * malloc doesn't initialize memory (contains garbage)
     * We explicitly set each element
     */
    for (int i = 0; i < 10; i++) {
        numbers[i] = i * i;  /* Store squares: 0, 1, 4, 9, ... */
    }
    
    /* Use the allocated memory */
    printf("Allocated array contents:\n");
    for (int i = 0; i < 10; i++) {
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }
    
    /* Print memory information */
    printf("\nMemory information:\n");
    printf("  Pointer address: %p\n", (void *)&numbers);
    printf("  Allocated block: %p\n", (void *)numbers);
    printf("  Block size: %zu bytes\n", 10 * sizeof(int));
    
    /* Free the allocated memory
     * This returns memory to the heap allocator
     * The pointer still holds the address (dangling pointer)
     */
    free(numbers);
    
    /* Set pointer to NULL after free
     * This prevents accidental use-after-free
     * free(NULL) is safe and does nothing
     */
    numbers = NULL;
    
    /* Verify cleanup */
    printf("\nMemory has been freed and pointer set to NULL\n");
    
    /* Demonstrate calloc (zero-initialized) */
    printf("\n--- Using calloc for zero-initialized memory ---\n");
    
    double *values = (double *)calloc(5, sizeof(double));
    if (values == NULL) {
        fprintf(stderr, "Error: Calloc allocation failed\n");
        return 1;
    }
    
    /* calloc initializes all bytes to zero */
    printf("Calloc'd array (should be all zeros):\n");
    for (int i = 0; i < 5; i++) {
        printf("values[%d] = %.2f\n", i, values[i]);
    }
    
    /* Now set some values */
    values[0] = 3.14159;
    values[1] = 2.71828;
    values[2] = 1.41421;
    values[3] = 1.73205;
    values[4] = 1.61803;
    
    printf("\nAfter setting values:\n");
    for (int i = 0; i < 5; i++) {
        printf("values[%d] = %.5f\n", i, values[i]);
    }
    
    free(values);
    values = NULL;
    
    printf("\nAll memory properly freed\n");
    
    return 0;
}
