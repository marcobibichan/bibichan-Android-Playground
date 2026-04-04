/**
 * ex02-parameter-passing.c
 * Purpose: Demonstrate call by value vs call by pointer
 * Dependencies: stdio.h
 * Compile: gcc -o ex02 ex02-parameter-passing.c
 * Run: ./ex02
 */

#include <stdio.h>

/* Call by value - cannot modify original */
void increment_by_value(int x) {
    x = x + 1;  /* Only modifies the copy */
    printf("Inside function: x = %d\n", x);
}

/* Call by pointer - can modify original */
void increment_by_pointer(int *x) {
    *x = *x + 1;  /* Modifies the value at the address */
    printf("Inside function: *x = %d\n", *x);
}

/* Swap two values using pointers */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Process array (array decays to pointer) */
void double_array(int arr[], size_t count) {
    for (size_t i = 0; i < count; i++) {
        arr[i] *= 2;  /* Can modify array elements */
    }
}

/* Const pointer parameter - cannot modify */
void print_array(const int *arr, size_t count) {
    printf("Array: ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int value = 5;
    
    /* Call by value demonstration */
    printf("=== Call by Value ===\n");
    printf("Before: value = %d\n", value);
    increment_by_value(value);
    printf("After: value = %d (unchanged)\n\n", value);
    
    /* Call by pointer demonstration */
    printf("=== Call by Pointer ===\n");
    printf("Before: value = %d\n", value);
    increment_by_pointer(&value);
    printf("After: value = %d (changed)\n\n", value);
    
    /* Swap demonstration */
    int a = 10, b = 20;
    printf("Before swap: a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("After swap: a = %d, b = %d\n\n", a, b);
    
    /* Array demonstration */
    int numbers[] = {1, 2, 3, 4, 5};
    size_t count = sizeof(numbers) / sizeof(numbers[0]);
    
    print_array(numbers, count);
    double_array(numbers, count);
    print_array(numbers, count);
    
    return 0;
}
