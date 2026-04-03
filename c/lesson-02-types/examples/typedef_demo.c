// File: typedef_demo.c
// Purpose: Demonstrate typedef for audio types
// Compile: gcc typedef_demo.c -o typedef_demo
// Run:     ./typedef_demo

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Basic typedef for audio samples
typedef int16_t sample16_t;
typedef int32_t sample32_t;
typedef float sample_float_t;

// Typedef for sizes
typedef size_t audio_size_t;
typedef uint32_t sample_rate_t;
typedef uint8_t channel_count_t;

// Struct typedef
typedef struct AudioBuffer {
    sample16_t *samples;
    audio_size_t size;
    audio_size_t capacity;
    sample_rate_t sample_rate;
    channel_count_t channels;
} AudioBuffer;

// Function pointer typedef
typedef void (*AudioCallback)(AudioBuffer *buffer, void *userdata);
typedef int (*ProcessFunc)(sample16_t *input, sample16_t *output, size_t count);

// Opaque type (forward declaration)
typedef struct AudioDecoder AudioDecoder;

// Error code typedef
typedef enum {
    AUDIO_OK = 0,
    AUDIO_ERROR_INVALID = -1,
    AUDIO_ERROR_MEMORY = -2,
    AUDIO_ERROR_IO = -3
} AudioError;

// Create audio buffer
AudioBuffer *audio_buffer_create(audio_size_t capacity, 
                                  sample_rate_t rate, 
                                  channel_count_t channels) {
    AudioBuffer *buf = malloc(sizeof(AudioBuffer));
    if (!buf) return NULL;
    
    buf->samples = malloc(capacity * sizeof(sample16_t));
    if (!buf->samples) {
        free(buf);
        return NULL;
    }
    
    buf->capacity = capacity;
    buf->size = 0;
    buf->sample_rate = rate;
    buf->channels = channels;
    
    return buf;
}

// Free audio buffer
void audio_buffer_free(AudioBuffer *buf) {
    if (buf) {
        free(buf->samples);
        free(buf);
    }
}

// Process audio samples
static int process_samples(sample16_t *input, sample16_t *output, size_t count) {
    for (size_t i = 0; i < count; i++) {
        // Simple gain adjustment (multiply by 0.5)
        output[i] = input[i] / 2;
    }
    return AUDIO_OK;
}

int main(void) {
    printf("=== Typedef Demo ===\n\n");
    
    // Using typedef'd types
    sample_rate_t rate = 192000;
    channel_count_t channels = 2;
    audio_size_t capacity = 4096;
    
    printf("Audio Configuration:\n");
    printf("  Sample Rate: %u Hz\n", rate);
    printf("  Channels: %u\n", channels);
    printf("  Buffer Capacity: %zu samples\n", capacity);
    
    // Create buffer
    AudioBuffer *buffer = audio_buffer_create(capacity, rate, channels);
    if (!buffer) {
        fprintf(stderr, "Failed to create buffer\n");
        return 1;
    }
    
    printf("\nBuffer created:\n");
    printf("  Capacity: %zu samples\n", buffer->capacity);
    printf("  Sample size: %zu bytes\n", sizeof(sample16_t));
    printf("  Total size: %zu bytes\n", buffer->capacity * sizeof(sample16_t));
    
    // Fill with test data
    for (audio_size_t i = 0; i < buffer->capacity; i++) {
        buffer->samples[i] = (sample16_t)(i % 65536 - 32768);
    }
    buffer->size = buffer->capacity;
    
    // Process using function pointer
    ProcessFunc processor = process_samples;
    sample16_t *output = malloc(capacity * sizeof(sample16_t));
    
    if (output) {
        AudioError result = processor(buffer->samples, output, capacity);
        printf("\nProcessing result: %d\n", result);
        printf("Input[0]: %d, Output[0]: %d\n", buffer->samples[0], output[0]);
        free(output);
    }
    
    // Cleanup
    audio_buffer_free(buffer);
    
    printf("\n=== Type Sizes ===\n\n");
    printf("  sizeof(sample16_t): %zu\n", sizeof(sample16_t));
    printf("  sizeof(sample32_t): %zu\n", sizeof(sample32_t));
    printf("  sizeof(sample_float_t): %zu\n", sizeof(sample_float_t));
    printf("  sizeof(AudioBuffer): %zu\n", sizeof(AudioBuffer));
    
    return 0;
}
