// File: sizeof_alignof.c
// Purpose: Demonstrate sizeof and alignof operators
// Compile: gcc sizeof_alignof.c -o sizeof_alignof
// Run:     ./sizeof_alignof

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>    // for offsetof
#include <stdalign.h>  // C11: alignof

// Struct with different alignments
struct AudioSample {
    int8_t channel;      // 1 byte
    int32_t value;       // 4 bytes (aligned to 4)
    int16_t gain;        // 2 bytes
};  // Total: likely 12 bytes due to padding

// Packed struct (no padding)
#pragma pack(push, 1)
struct PackedAudioSample {
    int8_t channel;      // 1 byte
    int32_t value;       // 4 bytes
    int16_t gain;        // 2 bytes
};  // Total: 7 bytes
#pragma pack(pop)

// Aligned struct
struct alignas(16) AlignedBuffer {
    uint8_t data[16];
};

int main(void) {
    printf("=== sizeof Operator ===\n\n");
    
    // Basic types
    printf("Basic types:\n");
    printf("  sizeof(char)      = %zu\n", sizeof(char));
    printf("  sizeof(short)     = %zu\n", sizeof(short));
    printf("  sizeof(int)       = %zu\n", sizeof(int));
    printf("  sizeof(long)      = %zu\n", sizeof(long));
    printf("  sizeof(long long) = %zu\n", sizeof(long long));
    printf("  sizeof(float)     = %zu\n", sizeof(float));
    printf("  sizeof(double)    = %zu\n", sizeof(double));
    printf("  sizeof(void*)     = %zu\n", sizeof(void*));
    
    // Fixed-width types
    printf("\nFixed-width types:\n");
    printf("  sizeof(int8_t)    = %zu\n", sizeof(int8_t));
    printf("  sizeof(int16_t)   = %zu\n", sizeof(int16_t));
    printf("  sizeof(int32_t)   = %zu\n", sizeof(int32_t));
    printf("  sizeof(int64_t)   = %zu\n", sizeof(int64_t));
    
    // Arrays
    printf("\nArrays:\n");
    int16_t samples[1024];
    printf("  int16_t samples[1024]: %zu bytes (%zu elements)\n", 
           sizeof(samples), sizeof(samples) / sizeof(samples[0]));
    
    // Pointers
    printf("\nPointers:\n");
    int *ptr;
    printf("  sizeof(int*)      = %zu (pointer size, not pointed data)\n", sizeof(ptr));
    
    printf("\n=== alignof Operator (C11) ===\n\n");
    
    // Basic alignments
    printf("Basic alignments:\n");
    printf("  alignof(char)      = %zu\n", alignof(char));
    printf("  alignof(short)     = %zu\n", alignof(short));
    printf("  alignof(int)       = %zu\n", alignof(int));
    printf("  alignof(double)    = %zu\n", alignof(double));
    printf("  alignof(int64_t)   = %zu\n", alignof(int64_t));
    
    printf("\n=== Struct Layout ===\n\n");
    
    // Normal struct
    printf("Normal struct AudioSample:\n");
    printf("  sizeof(struct AudioSample) = %zu\n", sizeof(struct AudioSample));
    printf("  alignof(struct AudioSample) = %zu\n", alignof(struct AudioSample));
    printf("  Offsets:\n");
    printf("    channel: %zu\n", offsetof(struct AudioSample, channel));
    printf("    value:   %zu\n", offsetof(struct AudioSample, value));
    printf("    gain:    %zu\n", offsetof(struct AudioSample, gain));
    
    // Packed struct
    printf("\nPacked struct PackedAudioSample:\n");
    printf("  sizeof(struct PackedAudioSample) = %zu\n", sizeof(struct PackedAudioSample));
    printf("  alignof(struct PackedAudioSample) = %zu\n", alignof(struct PackedAudioSample));
    
    // Aligned struct
    printf("\nAligned struct AlignedBuffer:\n");
    printf("  sizeof(struct AlignedBuffer) = %zu\n", sizeof(struct AlignedBuffer));
    printf("  alignof(struct AlignedBuffer) = %zu\n", alignof(struct AlignedBuffer));
    
    printf("\n=== Audio Buffer Calculations ===\n\n");
    
    // Calculate buffer sizes
    size_t sample_count = 4096;
    size_t buffer_size = sample_count * sizeof(int16_t);
    printf("Buffer for %zu samples (16-bit): %zu bytes (%.2f KB)\n", 
           sample_count, buffer_size, buffer_size / 1024.0);
    
    // Hi-Res buffer
    sample_count = 4096;
    buffer_size = sample_count * sizeof(int32_t);
    printf("Buffer for %zu samples (32-bit): %zu bytes (%.2f KB)\n", 
           sample_count, buffer_size, buffer_size / 1024.0);
    
    // Aligned allocation
    size_t alignment = 16;
    size_t aligned_size = (buffer_size + alignment - 1) & ~(alignment - 1);
    printf("Aligned buffer (16-byte): %zu bytes\n", aligned_size);
    
    return 0;
}
