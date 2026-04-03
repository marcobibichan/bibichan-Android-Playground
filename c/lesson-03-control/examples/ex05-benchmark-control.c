/*
 * ex05-benchmark-control.c - Performance benchmarks for control flow
 * Compile: gcc -O2 -Wall -Wextra -o benchmark_control ex05-benchmark-control.c
 * Run: ./benchmark_control
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define ITERATIONS (100 * 1000 * 1000)  // 100 million
#define ARRAY_SIZE (1024 * 1024)        // 1M elements

// ============================================================================
// Benchmark 1: Branch prediction impact
// ============================================================================
void benchmark_branch_prediction(void) {
    printf("\n=== Branch Prediction Benchmark ===\n");
    
    // Create predictable and unpredictable data
    int *predictable = malloc(ARRAY_SIZE * sizeof(int));
    int *unpredictable = malloc(ARRAY_SIZE * sizeof(int));
    
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        predictable[i] = (i % 4 == 0) ? 1 : 0;  // Predictable pattern
        unpredictable[i] = rand() % 4 == 0 ? 1 : 0;  // Random
    }
    
    volatile int64_t sum = 0;
    clock_t start, end;
    
    // Predictable branches
    start = clock();
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        if (predictable[i]) {
            sum += i;
        }
    }
    end = clock();
    printf("Predictable branches: %.3f seconds\n", 
           (double)(end - start) / CLOCKS_PER_SEC);
    
    // Unpredictable branches
    sum = 0;
    start = clock();
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        if (unpredictable[i]) {
            sum += i;
        }
    }
    end = clock();
    printf("Unpredictable branches: %.3f seconds\n", 
           (double)(end - start) / CLOCKS_PER_SEC);
    
    free(predictable);
    free(unpredictable);
}

// ============================================================================
// Benchmark 2: Switch vs if-else vs array lookup
// ============================================================================
int switch_func(int x) {
    switch (x) {
        case 0: return 10;
        case 1: return 20;
        case 2: return 30;
        case 3: return 40;
        case 4: return 50;
        case 5: return 60;
        case 6: return 70;
        case 7: return 80;
        case 8: return 90;
        case 9: return 100;
        default: return 0;
    }
}

int ifelse_func(int x) {
    if (x == 0) return 10;
    else if (x == 1) return 20;
    else if (x == 2) return 30;
    else if (x == 3) return 40;
    else if (x == 4) return 50;
    else if (x == 5) return 60;
    else if (x == 6) return 70;
    else if (x == 7) return 80;
    else if (x == 8) return 90;
    else if (x == 9) return 100;
    else return 0;
}

int array_func(int x) {
    static const int values[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    if (x >= 0 && x < 10) return values[x];
    return 0;
}

void benchmark_switch_ifelse_array(void) {
    printf("\n=== Switch vs If-Else vs Array Lookup ===\n");
    
    volatile int result = 0;
    clock_t start, end;
    
    // Switch benchmark
    start = clock();
    for (int64_t i = 0; i < ITERATIONS; i++) {
        result += switch_func(i % 10);
    }
    end = clock();
    printf("Switch: %.3f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    // If-else benchmark
    start = clock();
    for (int64_t i = 0; i < ITERATIONS; i++) {
        result += ifelse_func(i % 10);
    }
    end = clock();
    printf("If-else: %.3f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    // Array lookup benchmark
    start = clock();
    for (int64_t i = 0; i < ITERATIONS; i++) {
        result += array_func(i % 10);
    }
    end = clock();
    printf("Array lookup: %.3f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// ============================================================================
// Benchmark 3: Loop unrolling impact
// ============================================================================
void benchmark_loop_unrolling(void) {
    printf("\n=== Loop Unrolling Benchmark ===\n");
    
    int *data = malloc(ARRAY_SIZE * sizeof(int));
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        data[i] = rand();
    }
    
    volatile int64_t sum = 0;
    clock_t start, end;
    
    // Normal loop
    start = clock();
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        sum += data[i];
    }
    end = clock();
    printf("Normal loop: %.3f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    // Manual unrolling (4x)
    sum = 0;
    start = clock();
    size_t i;
    for (i = 0; i + 3 < ARRAY_SIZE; i += 4) {
        sum += data[i];
        sum += data[i + 1];
        sum += data[i + 2];
        sum += data[i + 3];
    }
    // Handle remaining elements
    for (; i < ARRAY_SIZE; i++) {
        sum += data[i];
    }
    end = clock();
    printf("Manual unroll (4x): %.3f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    free(data);
}

// ============================================================================
// Benchmark 4: Early exit vs full scan
// ============================================================================
void benchmark_early_exit(void) {
    printf("\n=== Early Exit Benchmark ===\n");
    
    int *data = malloc(ARRAY_SIZE * sizeof(int));
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        data[i] = i + 1;  // All positive
    }
    data[ARRAY_SIZE / 4] = -1;  // Target at 25%
    data[ARRAY_SIZE / 2] = -1;  // Target at 50%
    
    clock_t start, end;
    int found_index;
    
    // Early exit at 25%
    start = clock();
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        if (data[i] < 0) {
            found_index = i;
            break;
        }
    }
    end = clock();
    printf("Early exit (25%%): %.6f seconds, found at index %d\n", 
           (double)(end - start) / CLOCKS_PER_SEC, found_index);
    
    // Full scan (no break)
    start = clock();
    int last_negative = -1;
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        if (data[i] < 0) {
            last_negative = i;
        }
    }
    end = clock();
    printf("Full scan: %.6f seconds, last negative at %d\n", 
           (double)(end - start) / CLOCKS_PER_SEC, last_negative);
    
    free(data);
}

// ============================================================================
// Benchmark 5: Ternary vs if-else
// ============================================================================
void benchmark_ternary_vs_ifelse(void) {
    printf("\n=== Ternary vs If-Else Benchmark ===\n");
    
    int *data = malloc(ARRAY_SIZE * sizeof(int));
    int *result = malloc(ARRAY_SIZE * sizeof(int));
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        data[i] = rand() % 100;
    }
    
    clock_t start, end;
    
    // Ternary operator
    start = clock();
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        result[i] = (data[i] > 50) ? data[i] * 2 : data[i];
    }
    end = clock();
    printf("Ternary: %.3f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    // If-else
    start = clock();
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        if (data[i] > 50) {
            result[i] = data[i] * 2;
        } else {
            result[i] = data[i];
        }
    }
    end = clock();
    printf("If-else: %.3f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    free(data);
    free(result);
}

// ============================================================================
// Main
// ============================================================================
int main(void) {
    printf("=== Control Flow Performance Benchmarks ===\n");
    printf("Iterations: %d, Array size: %d\n", ITERATIONS, ARRAY_SIZE);
    
    benchmark_branch_prediction();
    benchmark_switch_ifelse_array();
    benchmark_loop_unrolling();
    benchmark_early_exit();
    benchmark_ternary_vs_ifelse();
    
    printf("\n=== Benchmarks Complete ===\n");
    return EXIT_SUCCESS;
}
