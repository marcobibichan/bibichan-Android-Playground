/*
 * ex05-audio-buffer-pool.c
 * Purpose: Demonstrate memory pool pattern for audio buffers
 * Dependencies: stdlib.h, stdio.h, stdalign.h
 * Compile: gcc -O2 -o ex05 ex05-audio-buffer-pool.c
 * Run: ./ex05
 * 
 * This example shows a simplified version of FFmpeg's buffer pool concept,
 * which is used for efficient audio frame management.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>
#include <string.h>
#include <stdint.h>

/* ============================================================
 * Configuration
 * ============================================================ */

/* Audio buffer configuration */
#define AUDIO_ALIGNMENT 32
#define MAX_POOL_SIZE 16
#define DEFAULT_FRAME_SIZE 4096
#define SAMPLE_RATE_192K 192000
#define SAMPLE_RATE_48K 48000

/* ============================================================
 * Data Structures
 * ============================================================ */

/* Audio buffer structure */
typedef struct AudioBuffer {
    int sample_rate;      /* Sample rate in Hz */
    int channels;         /* Number of channels */
    int nb_samples;       /* Current number of samples */
    int capacity;         /* Maximum samples capacity */
    alignas(AUDIO_ALIGNMENT) float *samples;  /* Sample data */
    struct AudioBuffer *next;  /* For pool linkage */
    int in_use;           /* Allocation flag */
    int64_t pts;          /* Presentation timestamp */
} AudioBuffer;

/* Buffer pool structure */
typedef struct BufferPool {
    AudioBuffer *buffers[MAX_POOL_SIZE];
    int count;            /* Number of buffers in pool */
    int sample_rate;      /* Default sample rate */
    int channels;         /* Default channels */
    int frame_size;       /* Default frame size */
    int hit_count;        /* Cache hits */
    int miss_count;       /* Cache misses */
} BufferPool;

/* ============================================================
 * Buffer Pool Implementation
 * ============================================================ */

/* Create a buffer pool */
BufferPool *buffer_pool_create(int sample_rate, int channels, int frame_size) {
    BufferPool *pool = (BufferPool *)calloc(1, sizeof(BufferPool));
    if (pool == NULL) {
        return NULL;
    }
    
    pool->sample_rate = sample_rate;
    pool->channels = channels;
    pool->frame_size = frame_size;
    pool->count = 0;
    pool->hit_count = 0;
    pool->miss_count = 0;
    
    return pool;
}

/* Allocate a single audio buffer */
static AudioBuffer *audio_buffer_alloc(int sample_rate, int channels, int nb_samples) {
    AudioBuffer *buf = (AudioBuffer *)aligned_alloc(AUDIO_ALIGNMENT, sizeof(AudioBuffer));
    if (buf == NULL) {
        return NULL;
    }
    
    /* Calculate aligned size for samples */
    size_t samples_size = channels * nb_samples * sizeof(float);
    if (samples_size % AUDIO_ALIGNMENT != 0) {
        samples_size += AUDIO_ALIGNMENT - (samples_size % AUDIO_ALIGNMENT);
    }
    
    buf->samples = (float *)aligned_alloc(AUDIO_ALIGNMENT, samples_size);
    if (buf->samples == NULL) {
        free(buf);
        return NULL;
    }
    
    buf->sample_rate = sample_rate;
    buf->channels = channels;
    buf->nb_samples = 0;
    buf->capacity = nb_samples;
    buf->next = NULL;
    buf->in_use = 0;
    buf->pts = 0;
    
    return buf;
}

/* Free a single audio buffer */
static void audio_buffer_free(AudioBuffer *buf) {
    if (buf != NULL) {
        free(buf->samples);
        free(buf);
    }
}

/* Get a buffer from the pool (or allocate new) */
AudioBuffer *buffer_pool_get(BufferPool *pool) {
    if (pool == NULL) {
        return NULL;
    }
    
    /* Try to find an unused buffer in the pool */
    for (int i = 0; i < pool->count; i++) {
        if (!pool->buffers[i]->in_use) {
            pool->buffers[i]->in_use = 1;
            pool->buffers[i]->nb_samples = 0;
            pool->buffers[i]->pts = 0;
            pool->hit_count++;
            return pool->buffers[i];
        }
    }
    
    /* No free buffer, allocate new if pool not full */
    if (pool->count < MAX_POOL_SIZE) {
        AudioBuffer *buf = audio_buffer_alloc(
            pool->sample_rate,
            pool->channels,
            pool->frame_size
        );
        if (buf != NULL) {
            buf->in_use = 1;
            pool->buffers[pool->count++] = buf;
            pool->miss_count++;
            return buf;
        }
    }
    
    /* Pool exhausted */
    fprintf(stderr, "Buffer pool exhausted\n");
    return NULL;
}

/* Return a buffer to the pool */
void buffer_pool_return(BufferPool *pool, AudioBuffer *buf) {
    if (pool == NULL || buf == NULL) {
        return;
    }
    
    /* Verify buffer belongs to this pool */
    for (int i = 0; i < pool->count; i++) {
        if (pool->buffers[i] == buf) {
            buf->in_use = 0;
            return;
        }
    }
    
    /* Buffer doesn't belong to pool, free it */
    fprintf(stderr, "Warning: Returning foreign buffer\n");
    audio_buffer_free(buf);
}

/* Destroy the buffer pool */
void buffer_pool_destroy(BufferPool *pool) {
    if (pool == NULL) {
        return;
    }
    
    for (int i = 0; i < pool->count; i++) {
        audio_buffer_free(pool->buffers[i]);
    }
    
    free(pool);
}

/* Get pool statistics */
void buffer_pool_stats(BufferPool *pool) {
    if (pool == NULL) return;
    
    printf("Pool Statistics:\n");
    printf("  Buffers allocated: %d / %d\n", pool->count, MAX_POOL_SIZE);
    printf("  Cache hits: %d\n", pool->hit_count);
    printf("  Cache misses: %d\n", pool->miss_count);
    if (pool->hit_count + pool->miss_count > 0) {
        printf("  Hit rate: %.1f%%\n", 
               100.0 * pool->hit_count / (pool->hit_count + pool->miss_count));
    }
}

/* ============================================================
 * Audio Processing Simulation
 * ============================================================ */

/* Simulate filling buffer with audio data */
void fill_audio_buffer(AudioBuffer *buf, int nb_samples, int64_t pts) {
    if (buf == NULL) return;
    
    buf->nb_samples = nb_samples;
    buf->pts = pts;
    
    /* Fill with test sine wave */
    for (int i = 0; i < buf->channels * buf->nb_samples; i++) {
        buf->samples[i] = (float)(i % 100) / 100.0f;
    }
}

/* Simulate audio processing */
void process_audio_buffer(AudioBuffer *buf) {
    if (buf == NULL) return;
    
    /* Apply simple gain */
    for (int i = 0; i < buf->channels * buf->nb_samples; i++) {
        buf->samples[i] *= 0.8f;
    }
}

/* Simulate audio playback */
void play_audio_buffer(AudioBuffer *buf) {
    if (buf == NULL) return;
    
    printf("Playing: pts=%ld, samples=%d, ch=%d, rate=%d\n",
           buf->pts, buf->nb_samples, buf->channels, buf->sample_rate);
}

/* ============================================================
 * Demo Functions
 * ============================================================ */

/* Simulate audio processing pipeline */
void simulate_audio_processing(BufferPool *pool, int num_frames) {
    printf("\n=== Simulating Audio Processing (%d frames) ===\n", num_frames);
    
    for (int frame = 0; frame < num_frames; frame++) {
        /* Get buffer from pool */
        AudioBuffer *buf = buffer_pool_get(pool);
        if (buf == NULL) {
            fprintf(stderr, "Failed to get buffer for frame %d\n", frame);
            continue;
        }
        
        /* Fill with audio data */
        fill_audio_buffer(buf, pool->frame_size, frame * pool->frame_size);
        
        /* Process */
        process_audio_buffer(buf);
        
        /* Play (in real app, would send to audio device) */
        if (frame % 5 == 0) {  /* Print every 5th frame */
            play_audio_buffer(buf);
        }
        
        /* Return buffer to pool for reuse */
        buffer_pool_return(pool, buf);
    }
    
    printf("\n");
    buffer_pool_stats(pool);
}

/* Demonstrate buffer pool efficiency */
void demonstrate_pool_efficiency(void) {
    printf("\n=== Buffer Pool Efficiency Demo ===\n");
    
    /* Create pool for Hi-Res audio */
    BufferPool *pool = buffer_pool_create(SAMPLE_RATE_192K, 2, DEFAULT_FRAME_SIZE);
    if (pool == NULL) {
        fprintf(stderr, "Failed to create buffer pool\n");
        return;
    }
    
    printf("Created pool for %d Hz, %d channels, %d samples/frame\n",
           SAMPLE_RATE_192K, 2, DEFAULT_FRAME_SIZE);
    printf("Memory per buffer: ~%zu bytes\n", 
           sizeof(AudioBuffer) + 2 * DEFAULT_FRAME_SIZE * sizeof(float));
    
    /* Process multiple batches to show reuse */
    for (int batch = 0; batch < 3; batch++) {
        printf("\n--- Batch %d ---\n", batch + 1);
        simulate_audio_processing(pool, 10);
    }
    
    buffer_pool_destroy(pool);
    printf("\nBuffer pool destroyed\n");
}

/* Demonstrate memory calculation for Hi-Res */
void demonstrate_hires_memory(void) {
    printf("\n=== Hi-Res Audio Memory Calculation ===\n");
    
    /* 192kHz/24-bit stereo FLAC stream */
    int sample_rate = 192000;
    int channels = 2;
    int bit_depth = 24;  /* Stored as 32-bit float */
    int frame_samples = 4096;
    int buffer_count = 16;
    
    printf("Stream parameters:\n");
    printf("  Sample rate: %d Hz\n", sample_rate);
    printf("  Channels: %d\n", channels);
    printf("  Bit depth: %d (stored as 32-bit float)\n", bit_depth);
    printf("  Frame size: %d samples\n", frame_samples);
    printf("  Buffer count: %d\n", buffer_count);
    
    /* Calculate memory */
    size_t bytes_per_sample = sizeof(float);  /* 4 bytes */
    size_t bytes_per_frame = frame_samples * channels * bytes_per_sample;
    size_t total_buffer_memory = bytes_per_frame * buffer_count;
    
    printf("\nMemory calculation:\n");
    printf("  Bytes per sample: %zu\n", bytes_per_sample);
    printf("  Bytes per frame: %zu (%.1f KB)\n", 
           bytes_per_frame, bytes_per_frame / 1024.0);
    printf("  Total buffer memory: %zu (%.1f KB)\n", 
           total_buffer_memory, total_buffer_memory / 1024.0);
    
    /* Playback duration per buffer */
    double duration_ms = (double)frame_samples / sample_rate * 1000.0;
    double total_duration_ms = duration_ms * buffer_count;
    
    printf("\nPlayback timing:\n");
    printf("  Duration per frame: %.2f ms\n", duration_ms);
    printf("  Total buffered: %.2f ms\n", total_duration_ms);
}

/* ============================================================
 * Main Function
 * ============================================================ */

int main(void) {
    printf("========================================\n");
    printf("Audio Buffer Pool Demo\n");
    printf("========================================\n");
    
    /* Show memory calculations */
    demonstrate_hires_memory();
    
    /* Demonstrate pool efficiency */
    demonstrate_pool_efficiency();
    
    /* Summary */
    printf("\n========================================\n");
    printf("Summary\n");
    printf("========================================\n");
    printf("Buffer pools provide:\n");
    printf("1. Reduced allocation overhead\n");
    printf("2. Better cache locality\n");
    printf("3. Predictable memory usage\n");
    printf("4. No fragmentation over time\n");
    printf("\nThis pattern is used in:\n");
    printf("- FFmpeg (AVFrame buffer pools)\n");
    printf("- Android NDK audio apps\n");
    printf("- Real-time audio processing\n");
    
    return 0;
}
