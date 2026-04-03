/*
 * ex01-if-basic.c - Basic conditional statements demonstration
 * Compile: gcc -Wall -Wextra -o if_basic ex01-if-basic.c
 * Run: ./if_basic
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Audio format detection using if-else
const char* detect_audio_format(int sample_rate, int bit_depth) {
    // First condition: check for Hi-Res audio
    // Hi-Res Audio is defined as > 44.1kHz and/or > 16-bit
    if (sample_rate >= 192000 && bit_depth >= 24) {
        return "Hi-Res Audio (192kHz/24bit)";
    }
    // Second condition: check for standard Hi-Res
    else if (sample_rate >= 96000 && bit_depth >= 24) {
        return "Hi-Res Audio (96kHz/24bit)";
    }
    // Third condition: CD quality
    else if (sample_rate >= 44100 && bit_depth >= 16) {
        return "CD Quality (44.1kHz/16bit)";
    }
    // Default case: low quality
    else {
        return "Low Quality Audio";
    }
}

// Audio codec selection based on file extension
void select_codec(const char *extension) {
    printf("Selecting codec for extension: %s\n", extension);
    
    // Nested if-else for codec selection
    if (extension != NULL) {
        // Compare extension with known formats
        if (strcmp(extension, "flac") == 0) {
            printf("  -> Using FLAC decoder (lossless)\n");
        }
        else if (strcmp(extension, "mp3") == 0) {
            printf("  -> Using MP3 decoder (lossy)\n");
        }
        else if (strcmp(extension, "wav") == 0) {
            printf("  -> Using WAV decoder (PCM)\n");
        }
        else {
            printf("  -> Unknown format, using default decoder\n");
        }
    }
    else {
        printf("  -> Error: NULL extension provided\n");
    }
}

// Demonstrate common if-else pitfalls
void demonstrate_pitfalls(void) {
    int x = 5;
    int y = 10;
    
    // Pitfall 1: Assignment in condition (should be comparison)
    // This is valid C but often a bug
    if (x = 3) {  // x is assigned 3, which is truthy
        printf("x is now %d (assignment in condition)\n", x);
    }
    
    // Correct way: use comparison
    if (y == 10) {
        printf("y is 10 (correct comparison)\n");
    }
    
    // Pitfall 2: Dangling else
    int a = 1, b = 0;
    if (a > 0)
        if (b > 0)
            printf("Both positive\n");
    else  // This else matches the inner if, not the outer!
        printf("a is not positive (WRONG!)\n");
    
    // Correct way: use braces
    if (a > 0) {
        if (b > 0) {
            printf("Both positive\n");
        }
    } else {
        printf("a is not positive (CORRECT)\n");
    }
}

int main(void) {
    printf("=== Basic Conditional Statements Demo ===\n\n");
    
    // Test audio format detection
    printf("Audio Format Detection:\n");
    printf("  192kHz/24bit: %s\n", detect_audio_format(192000, 24));
    printf("  96kHz/24bit: %s\n", detect_audio_format(96000, 24));
    printf("  44.1kHz/16bit: %s\n", detect_audio_format(44100, 16));
    printf("  22.05kHz/8bit: %s\n", detect_audio_format(22050, 8));
    
    printf("\nCodec Selection:\n");
    select_codec("flac");
    select_codec("mp3");
    select_codec("wav");
    select_codec("unknown");
    select_codec(NULL);
    
    printf("\nPitfall Demonstration:\n");
    demonstrate_pitfalls();
    
    return EXIT_SUCCESS;
}
