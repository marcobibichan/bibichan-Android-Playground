// File: ndk_audio_info.c
// Purpose: Display audio format information relevant to NDK development
// Compile: gcc ndk_audio_info.c -o ndk_audio_info
// NDK:     $NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang \
//          --target=aarch64-linux-android21 ndk_audio_info.c -o ndk_audio_info

#include <stdio.h>
#include <stdint.h>   // Fixed-width integer types
#include <limits.h>   // INT_MAX, etc.

// Audio format constants (relevant to Hi-Res audio)
#define SAMPLE_RATE_CD        44100
#define SAMPLE_RATE_HIRES     192000
#define SAMPLE_RATE_STUDIO    384000
#define BIT_DEPTH_CD          16
#define BIT_DEPTH_HIRES       24
#define BIT_DEPTH_FLOAT       32

// Calculate data rate
#define CALC_DATA_RATE(rate, depth, channels) ((rate) * (depth) * (channels) / 8)

int main(void) {
    printf("=== Audio Format Information ===\n\n");
    
    // CD Quality
    printf("CD Quality:\n");
    printf("  Sample Rate: %d Hz\n", SAMPLE_RATE_CD);
    printf("  Bit Depth: %d bits\n", BIT_DEPTH_CD);
    printf("  Data Rate (stereo): %d bytes/sec\n", 
           CALC_DATA_RATE(SAMPLE_RATE_CD, BIT_DEPTH_CD, 2));
    printf("  File size for 1 minute: %d bytes\n\n",
           CALC_DATA_RATE(SAMPLE_RATE_CD, BIT_DEPTH_CD, 2) * 60);
    
    // Hi-Res Quality
    printf("Hi-Res Quality (192kHz/24bit):\n");
    printf("  Sample Rate: %d Hz\n", SAMPLE_RATE_HIRES);
    printf("  Bit Depth: %d bits\n", BIT_DEPTH_HIRES);
    printf("  Data Rate (stereo): %d bytes/sec\n",
           CALC_DATA_RATE(SAMPLE_RATE_HIRES, BIT_DEPTH_HIRES, 2));
    printf("  File size for 1 minute: %d bytes\n\n",
           CALC_DATA_RATE(SAMPLE_RATE_HIRES, BIT_DEPTH_HIRES, 2) * 60);
    
    // Fixed-width types (important for NDK/JNI)
    printf("Fixed-width integer types (for NDK/JNI compatibility):\n");
    printf("  int8_t:   %zu bytes\n", sizeof(int8_t));
    printf("  int16_t:  %zu bytes\n", sizeof(int16_t));
    printf("  int32_t:  %zu bytes\n", sizeof(int32_t));
    printf("  int64_t:  %zu bytes\n", sizeof(int64_t));
    printf("  float:    %zu bytes\n", sizeof(float));
    printf("  double:   %zu bytes\n\n", sizeof(double));
    
    // Buffer size calculations
    printf("Buffer size recommendations:\n");
    int buffer_ms = 100;  // 100ms buffer
    int cd_buffer = CALC_DATA_RATE(SAMPLE_RATE_CD, BIT_DEPTH_CD, 2) * buffer_ms / 1000;
    int hires_buffer = CALC_DATA_RATE(SAMPLE_RATE_HIRES, BIT_DEPTH_HIRES, 2) * buffer_ms / 1000;
    printf("  CD 100ms buffer: %d bytes\n", cd_buffer);
    printf("  Hi-Res 100ms buffer: %d bytes\n", hires_buffer);
    
    return 0;
}
