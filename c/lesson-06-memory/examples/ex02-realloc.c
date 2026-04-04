/*
 * ex02-realloc.c
 * Purpose: Demonstrate dynamic array resizing with realloc
 * Dependencies: stdlib.h, stdio.h
 * Compile: gcc -o ex02 ex02-realloc.c
 * Run: ./ex02
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Dynamic array structure */
typedef struct {
    int *data;
    size_t size;      /* Current number of elements */
    size_t capacity;  /* Total allocated capacity */
} DynamicArray;

/* Create a new dynamic array */
DynamicArray *dyn_array_create(size_t initial_capacity) {
    DynamicArray *arr = (DynamicArray *)malloc(sizeof(DynamicArray));
    if (arr == NULL) {
        return NULL;
    }
    
    arr->data = (int *)malloc(initial_capacity * sizeof(int));
    if (arr->data == NULL) {
        free(arr);
        return NULL;
    }
    
    arr->size = 0;
    arr->capacity = initial_capacity;
    
    return arr;
}

/* Append element, growing array if necessary */
int dyn_array_append(DynamicArray *arr, int value) {
    if (arr == NULL) {
        return -1;
    }
    
    /* Check if we need more space */
    if (arr->size == arr->capacity) {
        /* Double the capacity (amortized O(1) append) */
        size_t new_capacity = arr->capacity * 2;
        
        /* CRITICAL: Use a temporary pointer for realloc
         * If realloc fails, original pointer is still valid
         */
        int *temp = (int *)realloc(arr->data, new_capacity * sizeof(int));
        
        if (temp == NULL) {
            fprintf(stderr, "Reallocation failed at size %zu\n", arr->size);
            return -1;  /* Original array is still valid */
        }
        
        /* realloc succeeded, update our pointer */
        arr->data = temp;
        arr->capacity = new_capacity;
        printf("Resized array from %zu to %zu elements\n", 
               arr->capacity / 2, arr->capacity);
    }
    
    /* Add element */
    arr->data[arr->size++] = value;
    return 0;
}

/* Get element at index */
int dyn_array_get(DynamicArray *arr, size_t index, int *out_value) {
    if (arr == NULL || index >= arr->size || out_value == NULL) {
        return -1;
    }
    *out_value = arr->data[index];
    return 0;
}

/* Free dynamic array */
void dyn_array_free(DynamicArray *arr) {
    if (arr != NULL) {
        free(arr->data);
        free(arr);
    }
}

/* Print array contents */
void dyn_array_print(DynamicArray *arr) {
    if (arr == NULL) {
        printf("Array is NULL\n");
        return;
    }
    
    printf("Array (size=%zu, capacity=%zu): [", arr->size, arr->capacity);
    for (size_t i = 0; i < arr->size; i++) {
        printf("%d", arr->data[i]);
        if (i < arr->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(void) {
    printf("=== Dynamic Array with realloc Demo ===\n\n");
    
    /* Create array with small initial capacity */
    size_t initial_capacity = 4;
    DynamicArray *arr = dyn_array_create(initial_capacity);
    
    if (arr == NULL) {
        fprintf(stderr, "Failed to create dynamic array\n");
        return 1;
    }
    
    printf("Created array with initial capacity %zu\n\n", initial_capacity);
    
    /* Add elements, watching reallocation */
    printf("Adding elements (watch for reallocation):\n");
    for (int i = 0; i < 20; i++) {
        if (dyn_array_append(arr, i * 10) != 0) {
            fprintf(stderr, "Failed to append element %d\n", i);
            break;
        }
    }
    
    printf("\n");
    dyn_array_print(arr);
    
    /* Demonstrate shrinking with realloc */
    printf("\n--- Shrinking array ---\n");
    printf("Current size: %zu, capacity: %zu\n", arr->size, arr->capacity);
    
    /* Shrink to exact size (not always recommended in practice) */
    size_t new_capacity = arr->size;
    int *temp = (int *)realloc(arr->data, new_capacity * sizeof(int));
    
    if (temp != NULL) {
        arr->data = temp;
        arr->capacity = new_capacity;
        printf("Shrunk capacity to %zu\n", arr->capacity);
    } else {
        printf("Shrink failed (original array still valid)\n");
    }
    
    dyn_array_print(arr);
    
    /* Demonstrate realloc with NULL (behaves like malloc) */
    printf("\n--- realloc with NULL (behaves like malloc) ---\n");
    int *new_arr = (int *)realloc(NULL, 5 * sizeof(int));
    if (new_arr != NULL) {
        printf("Allocated new array via realloc(NULL, ...)\n");
        for (int i = 0; i < 5; i++) {
            new_arr[i] = i * 100;
        }
        printf("New array: [%d, %d, %d, %d, %d]\n", 
               new_arr[0], new_arr[1], new_arr[2], new_arr[3], new_arr[4]);
        free(new_arr);
    }
    
    /* Demonstrate realloc with size 0 (behaves like free) */
    printf("\n--- realloc with size 0 (behaves like free) ---\n");
    int *to_free = (int *)malloc(100 * sizeof(int));
    if (to_free != NULL) {
        printf("Allocated array at %p\n", (void *)to_free);
        int *result = (int *)realloc(to_free, 0);
        printf("After realloc(ptr, 0): result=%p (implementation-defined)\n", 
               (void *)result);
        /* Note: Don't use to_free after this! */
    }
    
    /* Cleanup */
    printf("\n--- Cleanup ---\n");
    dyn_array_free(arr);
    printf("Dynamic array freed\n");
    
    return 0;
}
