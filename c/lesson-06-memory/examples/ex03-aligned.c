/*
 * ex03-aligned.c
 * Purpose: Demonstrate aligned memory allocation for SIMD operations
 * Dependencies: stdlib.h, stdio.h, stdalign.h
 * Compile: gcc -O2 -o ex03 ex03-aligned.c
 * Run: ./ex03
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>
#include <stdint.h>
#include <string.h>

/* AVX2 requires 32-byte alignment */
#define SIMD_ALIGNMENT_32 32
#define SIMD_ALIGNMENT_16 16
#define SIMD_ALIGNMENT_64 64

/* Check if pointer is aligned */
#define IS_ALIGNED(ptr, alignment) \
    (((uintptr_t)(ptr) & ((alignment) - 1)) == 0)

/* Helper to print alignment info */
void print_alignment_info(const char *name, void *ptr, size_t alignment) {
    printf("%-20s: address=%p, aligned=%s (required: %zu-byte)\n",
           name, ptr,
           IS_ALIGNED(ptr, alignment) ? "YES" : "NO",
           alignment);
}

/* Structure with alignment specification */
struct AlignedStruct {
    alignas(32) float data[16];  /* 32-byte aligned for AVX2 */
};

/* Structure without explicit alignment */
struct UnalignedStruct {
    float data[16];
};

int main(void) {
    printf("=== Memory Alignment Demo ===\n\n");
    
    /* Print alignment requirements for basic types */
    printf("--- Alignment Requirements ---\n");
    printf("char alignment:   %zu bytes\n", alignof(char));
    printf("short alignment:  %zu bytes\n", alignof(short));
    printf("int alignment:    %zu bytes\n", alignof(int));
    printf("long alignment:   %zu bytes\n", alignof(long));
    printf("float alignment:  %zu bytes\n", alignof(float));
    printf("double alignment: %zu bytes\n", alignof(double));
    printf("pointer alignment: %zu bytes\n", alignof(void*));
    printf("\n");
    
    /* Method 1: aligned_alloc (C11) */
    printf("--- Method 1: aligned_alloc() ---\n");
    
    /* Size must be a multiple of alignment for aligned_alloc */
    size_t buffer_size = 1024 * sizeof(float);
    size_t aligned_size = buffer_size;
    if (aligned_size % SIMD_ALIGNMENT_32 != 0) {
        aligned_size += SIMD_ALIGNMENT_32 - (aligned_size % SIMD_ALIGNMENT_32);
    }
    
    float *simd_buffer = (float *)aligned_alloc(SIMD_ALIGNMENT_32, aligned_size);
    
    if (simd_buffer == NULL) {
        fprintf(stderr, "Aligned allocation failed\n");
        return 1;
    }
    
    print_alignment_info("aligned_alloc buffer", simd_buffer, SIMD_ALIGNMENT_32);
    
    /* Initialize buffer (simulating audio samples) */
    for (size_t i = 0; i < 1024; i++) {
        simd_buffer[i] = (float)i / 1024.0f;
    }
    
    printf("First 8 samples: ");
    for (int i = 0; i < 8; i++) {
        printf("%.4f ", simd_buffer[i]);
    }
    printf("\n\n");
    
    free(simd_buffer);
    
    /* Method 2: Stack allocation with alignas */
    printf("--- Method 2: Stack allocation with alignas ---\n");
    
    alignas(SIMD_ALIGNMENT_32) float stack_buffer[1024];
    print_alignment_info("alignas stack buffer", stack_buffer, SIMD_ALIGNMENT_32);
    
    /* Method 3: Structure with alignas */
    printf("\n--- Method 3: Structure with alignas ---\n");
    
    struct AlignedStruct aligned_obj;
    struct UnalignedStruct unaligned_obj;
    
    print_alignment_info("AlignedStruct.data", aligned_obj.data, SIMD_ALIGNMENT_32);
    print_alignment_info("UnalignedStruct.data", unaligned_obj.data, SIMD_ALIGNMENT_32);
    
    printf("\nStructure sizes:\n");
    printf("AlignedStruct:   %zu bytes\n", sizeof(struct AlignedStruct));
    printf("UnalignedStruct: %zu bytes\n", sizeof(struct UnalignedStruct));
    
    /* Method 4: Manual alignment (posix_memalign style) */
    printf("\n--- Method 4: Manual alignment check ---\n");
    
    /* Allocate extra space for alignment */
    void *raw = malloc(1024 + SIMD_ALIGNMENT_64);
    if (raw == NULL) {
        fprintf(stderr, "Raw allocation failed\n");
        return 1;
    }
    
    /* Calculate aligned address */
    uintptr_t addr = (uintptr_t)raw;
    uintptr_t aligned_addr = (addr + SIMD_ALIGNMENT_64 - 1) & ~(SIMD_ALIGNMENT_64 - 1);
    void *aligned_ptr = (void *)aligned_addr;
    
    printf("Raw allocation:   %p\n", raw);
    printf("Aligned pointer:  %p\n", aligned_ptr);
    print_alignment_info("Manual aligned", aligned_ptr, SIMD_ALIGNMENT_64);
    
    free(raw);
    
    /* Demonstrate alignment impact on performance */
    printf("\n--- Alignment Impact on Cache ---\n");
    
    /* Cache line is typically 64 bytes */
    printf("Cache line size: 64 bytes (typical)\n");
    printf("Floats per cache line: %zu\n", 64 / sizeof(float));
    
    /* For best cache performance, access sequentially */
    alignas(64) float cache_optimized[1024];
    printf("Cache-optimized buffer: %p\n", (void *)cache_optimized);
    print_alignment_info("Cache-aligned", cache_optimized, 64);
    
    /* FFmpeg-style alignment requirements */
    printf("\n--- FFmpeg Alignment Requirements ---\n");
    printf("SSE/NEON:    16-byte alignment\n");
    printf("AVX/AVX2:    32-byte alignment\n");
    printf("AVX-512:     64-byte alignment\n");
    printf("Cache line:  64-byte alignment (for prefetching)\n");
    
    /* Allocate FFmpeg-style buffer */
    #define FF_ALIGN 32
    #define FF_BUFFER_SIZE (4096 * sizeof(float))
    
    size_t ff_size = FF_BUFFER_SIZE;
    if (ff_size % FF_ALIGN != 0) {
        ff_size += FF_ALIGN - (ff_size % FF_ALIGN);
    }
    
    float *ffmpeg_buffer = (float *)aligned_alloc(FF_ALIGN, ff_size);
    if (ffmpeg_buffer != NULL) {
        printf("\nFFmpeg-style buffer: %p (%zu bytes, %zu-byte aligned)\n",
               (void *)ffmpeg_buffer, ff_size, FF_ALIGN);
        print_alignment_info("FFmpeg buffer", ffmpeg_buffer, FF_ALIGN);
        free(ffmpeg_buffer);
    }
    
    printf("\nAll aligned allocations freed successfully\n");
    
    return 0;
}
