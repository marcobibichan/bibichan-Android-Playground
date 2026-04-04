/**
 * ex04-recursion-inline.c
 * Purpose: Demonstrate recursive functions and inline functions
 * Dependencies: stdio.h, stdint.h
 * Compile: gcc -O2 -o ex04 ex04-recursion-inline.c
 * Run: ./ex04
 */

#include <stdio.h>
#include <stdint.h>

/* Recursive function: factorial */
uint64_t factorial(int n) {
    if (n <= 1) {  /* Base case */
        return 1;
    }
    return n * factorial(n - 1);  /* Recursive case */
}

/* Recursive function: Fibonacci (inefficient) */
uint64_t fibonacci_naive(int n) {
    if (n <= 1) return n;
    return fibonacci_naive(n - 1) + fibonacci_naive(n - 2);
}

/* Iterative Fibonacci (efficient) */
uint64_t fibonacci_iterative(int n) {
    if (n <= 1) return n;
    
    uint64_t prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        uint64_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

/* Inline function for audio sample clamping */
static inline int32_t clamp_sample(int32_t sample, int32_t min_val, int32_t max_val) {
    if (sample < min_val) return min_val;
    if (sample > max_val) return max_val;
    return sample;
}

/* Inline function for sample conversion */
static inline float sample_to_float(int16_t sample) {
    return sample / 32768.0f;
}

/* Recursive binary search */
int binary_search(const int *arr, int left, int right, int target) {
    if (left > right) return -1;  /* Not found */
    
    int mid = left + (right - left) / 2;
    
    if (arr[mid] == target) return mid;
    if (arr[mid] > target) {
        return binary_search(arr, left, mid - 1, target);
    }
    return binary_search(arr, mid + 1, right, target);
}

/* Tail-recursive sum (can be optimized by compiler) */
int tail_sum(int n, int accumulator) {
    if (n == 0) return accumulator;
    return tail_sum(n - 1, accumulator + n);
}

int main(void) {
    printf("=== Recursive Functions ===\n");
    
    /* Factorial */
    for (int i = 0; i <= 10; i++) {
        printf("%d! = %lu\n", i, factorial(i));
    }
    printf("\n");
    
    /* Fibonacci comparison */
    printf("Fibonacci(10) naive: %lu\n", fibonacci_naive(10));
    printf("Fibonacci(10) iterative: %lu\n", fibonacci_iterative(10));
    printf("Fibonacci(40) iterative: %lu\n", fibonacci_iterative(40));
    printf("\n");
    
    printf("=== Inline Functions ===\n");
    
    /* Audio sample processing */
    int32_t samples[] = {-40000, 0, 20000, 40000};
    for (int i = 0; i < 4; i++) {
        int32_t clamped = clamp_sample(samples[i], -32768, 32767);
        float normalized = sample_to_float((int16_t)clamped);
        printf("Sample %d: %d -> %d -> %.6f\n", 
               i, samples[i], clamped, normalized);
    }
    printf("\n");
    
    printf("=== Binary Search ===\n");
    int sorted_arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int size = sizeof(sorted_arr) / sizeof(sorted_arr[0]);
    
    int targets[] = {7, 2, 19};
    for (int i = 0; i < 3; i++) {
        int idx = binary_search(sorted_arr, 0, size - 1, targets[i]);
        if (idx >= 0) {
            printf("Found %d at index %d\n", targets[i], idx);
        } else {
            printf("%d not found\n", targets[i]);
        }
    }
    printf("\n");
    
    printf("=== Tail Recursion ===\n");
    printf("Sum 1 to 100: %d\n", tail_sum(100, 0));
    
    return 0;
}
