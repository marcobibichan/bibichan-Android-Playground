// File: type_sizes.c
// Purpose: Display sizes and ranges of various C types
// Compile: gcc type_sizes.c -o type_sizes
// Run:     ./type_sizes

#include <stdio.h>
#include <stdint.h>    // Fixed-width types
#include <limits.h>    // Integer limits
#include <float.h>     // Floating-point limits

int main(void) {
    printf("=== Integer Type Sizes ===\n\n");
    
    // Basic integer types
    printf("Basic types:\n");
    printf("  char:       %2zu bytes, range: %d to %d\n", 
           sizeof(char), CHAR_MIN, CHAR_MAX);
    printf("  short:      %2zu bytes, range: %d to %d\n", 
           sizeof(short), SHRT_MIN, SHRT_MAX);
    printf("  int:        %2zu bytes, range: %d to %d\n", 
           sizeof(int), INT_MIN, INT_MAX);
    printf("  long:       %2zu bytes, range: %ld to %ld\n", 
           sizeof(long), LONG_MIN, LONG_MAX);
    printf("  long long:  %2zu bytes, range: %lld to %lld\n", 
           sizeof(long long), LLONG_MIN, LLONG_MAX);
    
    printf("\nUnsigned types:\n");
    printf("  unsigned char:      %2zu bytes, max: %u\n", 
           sizeof(unsigned char), UCHAR_MAX);
    printf("  unsigned short:     %2zu bytes, max: %u\n", 
           sizeof(unsigned short), USHRT_MAX);
    printf("  unsigned int:       %2zu bytes, max: %u\n", 
           sizeof(unsigned int), UINT_MAX);
    printf("  unsigned long:      %2zu bytes, max: %lu\n", 
           sizeof(unsigned long), ULONG_MAX);
    printf("  unsigned long long: %2zu bytes, max: %llu\n", 
           sizeof(unsigned long long), ULLONG_MAX);
    
    printf("\n=== Fixed-Width Types ===\n\n");
    
    printf("Signed fixed-width:\n");
    printf("  int8_t:   %zu bytes, range: %d to %d\n", 
           sizeof(int8_t), INT8_MIN, INT8_MAX);
    printf("  int16_t:  %zu bytes, range: %d to %d\n", 
           sizeof(int16_t), INT16_MIN, INT16_MAX);
    printf("  int32_t:  %zu bytes, range: %d to %d\n", 
           sizeof(int32_t), INT32_MIN, INT32_MAX);
    printf("  int64_t:  %zu bytes, range: %lld to %lld\n", 
           sizeof(int64_t), INT64_MIN, INT64_MAX);
    
    printf("\nUnsigned fixed-width:\n");
    printf("  uint8_t:  %zu bytes, max: %u\n", sizeof(uint8_t), UINT8_MAX);
    printf("  uint16_t: %zu bytes, max: %u\n", sizeof(uint16_t), UINT16_MAX);
    printf("  uint32_t: %zu bytes, max: %u\n", sizeof(uint32_t), UINT32_MAX);
    printf("  uint64_t: %zu bytes, max: %llu\n", sizeof(uint64_t), UINT64_MAX);
    
    printf("\n=== Floating-Point Types ===\n\n");
    
    printf("  float:       %zu bytes, range: %e to %e, precision: %d digits\n", 
           sizeof(float), FLT_MIN, FLT_MAX, FLT_DIG);
    printf("  double:      %zu bytes, range: %e to %e, precision: %d digits\n", 
           sizeof(double), DBL_MIN, DBL_MAX, DBL_DIG);
    printf("  long double: %zu bytes, range: %Le to %Le, precision: %d digits\n", 
           sizeof(long double), LDBL_MIN, LDBL_MAX, LDBL_DIG);
    
    printf("\n=== Audio-Relevant Calculations ===\n\n");
    
    // CD Quality
    int32_t cd_samples = 44100 * 2 * 60;  // 1 minute of CD audio
    size_t cd_bytes = cd_samples * sizeof(int16_t);
    printf("CD Quality (44.1kHz, 16-bit, stereo, 1 min):\n");
    printf("  Samples: %d\n", cd_samples);
    printf("  Bytes: %zu (%.2f KB)\n", cd_bytes, cd_bytes / 1024.0);
    
    // Hi-Res Quality
    int64_t hires_samples = 192000LL * 2 * 60;  // 1 minute of Hi-Res
    size_t hires_bytes = hires_samples * sizeof(int32_t);  // 24-bit in 32-bit
    printf("\nHi-Res Quality (192kHz, 24-bit, stereo, 1 min):\n");
    printf("  Samples: %lld\n", hires_samples);
    printf("  Bytes: %zu (%.2f MB)\n", hires_bytes, hires_bytes / (1024.0 * 1024.0));
    
    return 0;
}
