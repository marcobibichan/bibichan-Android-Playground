/*
 * ex04-memory-errors.c
 * Purpose: Demonstrate common memory errors and their detection
 * Dependencies: stdlib.h, stdio.h
 * Compile with sanitizers: gcc -fsanitize=address -g -o ex04 ex04-memory-errors.c
 * Run: ./ex04
 * 
 * WARNING: This program intentionally contains bugs for educational purposes!
 * Compile with AddressSanitizer to detect them.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * CORRECT IMPLEMENTATIONS (Always run these)
 * ============================================================ */

/* Correct memory usage pattern */
void correct_memory_usage(void) {
    printf("=== Correct Memory Usage ===\n");
    
    int *ptr = (int *)malloc(5 * sizeof(int));
    if (ptr == NULL) {
        fprintf(stderr, "Allocation failed\n");
        return;
    }
    
    /* Initialize */
    for (int i = 0; i < 5; i++) {
        ptr[i] = i * 10;
    }
    
    /* Use */
    for (int i = 0; i < 5; i++) {
        printf("ptr[%d] = %d\n", i, ptr[i]);
    }
    
    /* Cleanup */
    free(ptr);
    ptr = NULL;
    
    printf("Correct usage completed successfully\n\n");
}

/* Correct buffer handling */
void correct_buffer_handling(void) {
    printf("=== Correct Buffer Handling ===\n");
    
    char *buffer = (char *)malloc(10);
    if (buffer == NULL) {
        fprintf(stderr, "Allocation failed\n");
        return;
    }
    
    /* Safe string copy */
    strncpy(buffer, "Hello", 9);
    buffer[9] = '\0';  /* Ensure null termination */
    printf("Buffer: %s\n", buffer);
    
    free(buffer);
    buffer = NULL;
    
    printf("Correct buffer handling completed\n\n");
}

/* Correct realloc pattern */
void correct_realloc_pattern(void) {
    printf("=== Correct Realloc Pattern ===\n");
    
    size_t size = 5;
    int *arr = (int *)malloc(size * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Initial allocation failed\n");
        return;
    }
    
    /* Initialize */
    for (size_t i = 0; i < size; i++) {
        arr[i] = (int)i;
    }
    
    /* Grow array - use temp pointer */
    size_t new_size = 10;
    int *temp = (int *)realloc(arr, new_size * sizeof(int));
    if (temp == NULL) {
        fprintf(stderr, "Reallocation failed\n");
        free(arr);  /* Original still valid, clean up */
        return;
    }
    arr = temp;  /* Use new pointer */
    
    /* Initialize new elements */
    for (size_t i = size; i < new_size; i++) {
        arr[i] = (int)i * 2;
    }
    
    printf("Array after realloc: ");
    for (size_t i = 0; i < new_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    free(arr);
    arr = NULL;
    
    printf("Correct realloc pattern completed\n\n");
}

/* Correct nested allocation */
void correct_nested_allocation(void) {
    printf("=== Correct Nested Allocation ===\n");
    
    /* Allocate outer structure */
    typedef struct {
        int *data;
        size_t size;
    } DataStruct;
    
    DataStruct *obj = (DataStruct *)malloc(sizeof(DataStruct));
    if (obj == NULL) {
        fprintf(stderr, "Outer allocation failed\n");
        return;
    }
    
    /* Allocate inner data */
    obj->size = 10;
    obj->data = (int *)malloc(obj->size * sizeof(int));
    if (obj->data == NULL) {
        fprintf(stderr, "Inner allocation failed\n");
        free(obj);  /* Clean up outer */
        return;
    }
    
    /* Initialize */
    for (size_t i = 0; i < obj->size; i++) {
        obj->data[i] = (int)i * 100;
    }
    
    printf("Nested data: ");
    for (size_t i = 0; i < obj->size; i++) {
        printf("%d ", obj->data[i]);
    }
    printf("\n");
    
    /* Free in reverse order */
    free(obj->data);  /* Free inner first */
    free(obj);        /* Then outer */
    
    printf("Correct nested allocation completed\n\n");
}

/* ============================================================
 * ERROR DEMONSTRATIONS (Disabled by default)
 * Enable by defining macros at compile time
 * ============================================================ */

#ifdef DEMONSTRATE_OVERFLOW
void buffer_overflow_demo(void) {
    printf("=== Buffer Overflow (DEMONSTRATION) ===\n");
    
    int *arr = (int *)malloc(5 * sizeof(int));
    if (arr == NULL) return;
    
    /* BUG: Writing beyond allocated size */
    printf("Writing 10 elements to 5-element array...\n");
    for (int i = 0; i < 10; i++) {  /* Should be i < 5 */
        arr[i] = i;  /* Overflow when i >= 5 */
    }
    
    /* This may crash or corrupt memory */
    printf("Array contents (undefined): ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    free(arr);
}
#endif

#ifdef DEMONSTRATE_UAF
void use_after_free_demo(void) {
    printf("=== Use-After-Free (DEMONSTRATION) ===\n");
    
    int *ptr = (int *)malloc(sizeof(int));
    if (ptr == NULL) return;
    
    *ptr = 42;
    printf("Before free: %d\n", *ptr);
    
    free(ptr);
    
    /* BUG: Using pointer after free */
    printf("After free: %d (UNDEFINED BEHAVIOR)\n", *ptr);
    /* This may print garbage, crash, or appear to work */
}
#endif

#ifdef DEMONSTRATE_LEAK
void memory_leak_demo(void) {
    printf("=== Memory Leak (DEMONSTRATION) ===\n");
    
    int *leaked = (int *)malloc(1000 * sizeof(int));
    if (leaked == NULL) return;
    
    /* Initialize */
    for (int i = 0; i < 1000; i++) {
        leaked[i] = i;
    }
    
    printf("Allocated and initialized 1000 integers\n");
    printf("Function returns without freeing - MEMORY LEAK!\n");
    /* Missing: free(leaked); */
}
#endif

#ifdef DEMONSTRATE_DOUBLE_FREE
void double_free_demo(void) {
    printf("=== Double Free (DEMONSTRATION) ===\n");
    
    int *ptr = (int *)malloc(sizeof(int));
    if (ptr == NULL) return;
    
    *ptr = 42;
    printf("Value: %d\n", *ptr);
    
    free(ptr);
    printf("First free completed\n");
    
    /* BUG: Freeing same pointer twice */
    free(ptr);  /* UNDEFINED BEHAVIOR */
    printf("Second free completed (SHOULD NOT REACH HERE)\n");
}
#endif

#ifdef DEMONSTRATE_INVALID_FREE
void invalid_free_demo(void) {
    printf("=== Invalid Free (DEMONSTRATION) ===\n");
    
    int stack_var = 42;
    printf("Stack variable at: %p\n", (void *)&stack_var);
    
    /* BUG: Freeing stack memory */
    free(&stack_var);  /* UNDEFINED BEHAVIOR */
    printf("Invalid free completed (SHOULD NOT REACH HERE)\n");
}
#endif

#ifdef DEMONSTRATE_UNINITIALIZED
void uninitialized_read_demo(void) {
    printf("=== Uninitialized Read (DEMONSTRATION) ===\n");
    
    int *arr = (int *)malloc(5 * sizeof(int));
    if (arr == NULL) return;
    
    /* BUG: Reading uninitialized memory */
    printf("Uninitialized values (garbage):\n");
    for (int i = 0; i < 5; i++) {
        printf("arr[%d] = %d (garbage)\n", i, arr[i]);
    }
    
    free(arr);
}
#endif

/* ============================================================
 * MAIN FUNCTION
 * ============================================================ */

int main(void) {
    printf("========================================\n");
    printf("Memory Error Detection Demo\n");
    printf("========================================\n\n");
    
    /* Always run correct examples */
    correct_memory_usage();
    correct_buffer_handling();
    correct_realloc_pattern();
    correct_nested_allocation();
    
    printf("========================================\n");
    printf("Error Demonstrations\n");
    printf("========================================\n\n");
    
    /* Error demonstrations are disabled by default
     * Enable by defining macros at compile time:
     * 
     * gcc -DDEMONSTRATE_OVERFLOW -fsanitize=address -g -o ex04 ex04-memory-errors.c
     * gcc -DDEMONSTRATE_UAF -fsanitize=address -g -o ex04 ex04-memory-errors.c
     * gcc -DDEMONSTRATE_LEAK -fsanitize=leak -g -o ex04 ex04-memory-errors.c
     * gcc -DDEMONSTRATE_DOUBLE_FREE -fsanitize=address -g -o ex04 ex04-memory-errors.c
     * gcc -DDEMONSTRATE_INVALID_FREE -fsanitize=address -g -o ex04 ex04-memory-errors.c
     * gcc -DDEMONSTRATE_UNINITIALIZED -fsanitize=memory -g -o ex04 ex04-memory-errors.c
     */
    
    printf("Error demonstrations are DISABLED by default.\n\n");
    
    printf("To see error detection, compile with:\n");
    printf("  gcc -DDEMONSTRATE_OVERFLOW -fsanitize=address -g -o ex04 ex04-memory-errors.c\n");
    printf("  gcc -DDEMONSTRATE_UAF -fsanitize=address -g -o ex04 ex04-memory-errors.c\n");
    printf("  gcc -DDEMONSTRATE_LEAK -fsanitize=leak -g -o ex04 ex04-memory-errors.c\n");
    printf("  gcc -DDEMONSTRATE_DOUBLE_FREE -fsanitize=address -g -o ex04 ex04-memory-errors.c\n");
    printf("  gcc -DDEMONSTRATE_INVALID_FREE -fsanitize=address -g -o ex04 ex04-memory-errors.c\n");
    printf("  gcc -DDEMONSTRATE_UNINITIALIZED -fsanitize=memory -g -o ex04 ex04-memory-errors.c\n\n");
    
    printf("Or use valgrind:\n");
    printf("  gcc -g -o ex04 ex04-memory-errors.c\n");
    printf("  valgrind --leak-check=full ./ex04\n\n");
    
    printf("========================================\n");
    printf("Best Practices Summary\n");
    printf("========================================\n");
    printf("1. Always check malloc return value\n");
    printf("2. Initialize memory after allocation\n");
    printf("3. Set pointers to NULL after free\n");
    printf("4. Use sizeof for allocation sizes\n");
    printf("5. Free in reverse order of allocation\n");
    printf("6. Use temp pointer for realloc\n");
    printf("7. Test with sanitizers and valgrind\n");
    
    return 0;
}
