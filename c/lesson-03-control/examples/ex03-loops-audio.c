/*
 * ex03-loops-audio.c - Loop statements for audio processing
 * Compile: gcc -Wall -Wextra -lm -o loops_audio ex03-loops-audio.c
 * Run: ./loops_audio
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Simulated audio buffer structure
typedef struct {
    int32_t *samples;       // Sample data (24-bit stored in 32-bit)
    size_t num_samples;     // Number of samples
    int channels;           // Number of channels
    int sample_rate;        // Sample rate in Hz
} AudioBuffer;

// Create audio buffer
AudioBuffer* create_audio_buffer(size_t num_samples, int channels, int sample_rate) {
    AudioBuffer *buf = malloc(sizeof(AudioBuffer));
    if (!buf) return NULL;
    
    buf->samples = malloc(num_samples * channels * sizeof(int32_t));
    if (!buf->samples) {
        free(buf);
        return NULL;
    }
    
    buf->num_samples = num_samples;
    buf->channels = channels;
    buf->sample_rate = sample_rate;
    
    return buf;
}

// Free audio buffer
void free_audio_buffer(AudioBuffer *buf) {
    if (buf) {
        free(buf->samples);
        free(buf);
    }
}

// Generate test audio signal (sine wave)
void generate_sine_wave(AudioBuffer *buf, double frequency, double amplitude) {
    // For loop: iterate through all samples
    // This is the most common pattern for audio processing
    for (size_t i = 0; i < buf->num_samples; i++) {
        double t = (double)i / buf->sample_rate;
        double value = amplitude * sin(2.0 * M_PI * frequency * t);
        
        // Convert to 24-bit integer range
        buf->samples[i] = (int32_t)(value * 8388607.0);  // 2^23 - 1
    }
}

// Calculate peak amplitude using while loop
double calculate_peak_amplitude(const AudioBuffer *buf) {
    double peak = 0.0;
    size_t i = 0;
    
    // While loop: useful when condition is complex
    while (i < buf->num_samples) {
        double abs_val = fabs((double)buf->samples[i] / 8388607.0);
        if (abs_val > peak) {
            peak = abs_val;
        }
        i++;
    }
    
    return peak;
}

// Calculate RMS (Root Mean Square) level
double calculate_rms(const AudioBuffer *buf) {
    double sum_squares = 0.0;
    
    // For loop with accumulated calculation
    for (size_t i = 0; i < buf->num_samples; i++) {
        double normalized = (double)buf->samples[i] / 8388607.0;
        sum_squares += normalized * normalized;
    }
    
    return sqrt(sum_squares / buf->num_samples);
}

// Apply gain using do-while (demonstrating at-least-once execution)
void apply_gain(AudioBuffer *buf, double gain) {
    if (buf->num_samples == 0) return;
    
    size_t i = 0;
    
    // Do-while: ensures at least one iteration
    // Useful when you need to process first element unconditionally
    do {
        buf->samples[i] = (int32_t)(buf->samples[i] * gain);
        i++;
    } while (i < buf->num_samples);
}

// Find first sample above threshold (using break)
size_t find_first_above_threshold(const AudioBuffer *buf, double threshold) {
    // Early exit pattern using break
    for (size_t i = 0; i < buf->num_samples; i++) {
        double abs_val = fabs((double)buf->samples[i] / 8388607.0);
        if (abs_val > threshold) {
            return i;  // Found, exit early
        }
    }
    return buf->num_samples;  // Not found
}

// Count samples in range (using continue)
size_t count_samples_in_range(const AudioBuffer *buf, double min_val, double max_val) {
    size_t count = 0;
    
    for (size_t i = 0; i < buf->num_samples; i++) {
        double normalized = (double)buf->samples[i] / 8388607.0;
        
        // Skip samples outside range using continue
        if (normalized < min_val || normalized > max_val) {
            continue;  // Skip to next iteration
        }
        
        count++;
    }
    
    return count;
}

// Process multi-channel audio (nested loops)
void process_multichannel(AudioBuffer *buf) {
    printf("Processing %d channels, %zu samples per channel\n",
           buf->channels, buf->num_samples / buf->channels);
    
    // Outer loop: channels
    for (int ch = 0; ch < buf->channels; ch++) {
        // Inner loop: samples
        for (size_t i = 0; i < buf->num_samples / buf->channels; i++) {
            size_t idx = ch * (buf->num_samples / buf->channels) + i;
            // Process sample at buf->samples[idx]
        }
    }
}

// Simulate FFmpeg decode loop
void simulate_ffmpeg_decode_loop(void) {
    printf("\n=== Simulated FFmpeg Decode Loop ===\n");
    
    // Simulated packet counter
    int packet_count = 0;
    int max_packets = 10;
    int error_count = 0;
    
    // Typical FFmpeg decode pattern
    while (packet_count < max_packets) {
        printf("Processing packet %d...\n", packet_count);
        
        // Simulate decode result
        int ret = (rand() % 10 == 0) ? -1 : 0;  // 10% error rate
        
        if (ret < 0) {
            printf("  Error decoding packet, skipping...\n");
            error_count++;
            
            if (error_count > 3) {
                printf("  Too many errors, stopping decode\n");
                break;  // Exit on too many errors
            }
            continue;  // Skip to next packet
        }
        
        // Simulate frame processing
        int frames_processed = 0;
        while (1) {
            // Simulate receiving frame
            ret = (frames_processed >= 2) ? -1 : 0;  // EOF after 2 frames
            
            if (ret < 0) {
                break;  // No more frames
            }
            
            printf("  Processed frame %d\n", frames_processed);
            frames_processed++;
        }
        
        packet_count++;
    }
    
    printf("Decode complete: %d packets, %d errors\n", packet_count, error_count);
}

// Benchmark different loop patterns
void benchmark_loop_patterns(void) {
    printf("\n=== Loop Pattern Benchmarks ===\n");
    
    const size_t size = 10000000;
    int32_t *data = malloc(size * sizeof(int32_t));
    if (!data) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize data
    for (size_t i = 0; i < size; i++) {
        data[i] = rand();
    }
    
    clock_t start, end;
    volatile int64_t sum = 0;
    
    // Benchmark: for loop
    start = clock();
    for (size_t i = 0; i < size; i++) {
        sum += data[i];
    }
    end = clock();
    printf("For loop: %.3f seconds (sum=%ld)\n", 
           (double)(end - start) / CLOCKS_PER_SEC, sum);
    
    // Benchmark: while loop
    sum = 0;
    size_t i = 0;
    start = clock();
    while (i < size) {
        sum += data[i];
        i++;
    }
    end = clock();
    printf("While loop: %.3f seconds (sum=%ld)\n", 
           (double)(end - start) / CLOCKS_PER_SEC, sum);
    
    // Benchmark: do-while loop
    sum = 0;
    i = 0;
    start = clock();
    if (size > 0) {
        do {
            sum += data[i];
            i++;
        } while (i < size);
    }
    end = clock();
    printf("Do-while loop: %.3f seconds (sum=%ld)\n", 
           (double)(end - start) / CLOCKS_PER_SEC, sum);
    
    free(data);
}

int main(void) {
    printf("=== Loop Statements for Audio Processing ===\n\n");
    
    srand(time(NULL));
    
    // Create test audio buffer
    const size_t num_samples = 192000;  // 1 second at 192kHz
    AudioBuffer *buf = create_audio_buffer(num_samples, 2, 192000);
    if (!buf) {
        printf("Failed to create audio buffer\n");
        return EXIT_FAILURE;
    }
    
    // Generate test signal
    printf("Generating 1kHz sine wave at 192kHz sample rate...\n");
    generate_sine_wave(buf, 1000.0, 0.8);
    
    // Calculate metrics
    printf("Peak amplitude: %.4f\n", calculate_peak_amplitude(buf));
    printf("RMS level: %.4f\n", calculate_rms(buf));
    
    // Find threshold crossing
    size_t first_above = find_first_above_threshold(buf, 0.7);
    printf("First sample above 0.7 threshold: index %zu\n", first_above);
    
    // Count samples in range
    size_t in_range = count_samples_in_range(buf, -0.5, 0.5);
    printf("Samples in range [-0.5, 0.5]: %zu (%.1f%%)\n", 
           in_range, 100.0 * in_range / num_samples);
    
    // Simulate FFmpeg decode
    simulate_ffmpeg_decode_loop();
    
    // Benchmark
    benchmark_loop_patterns();
    
    // Cleanup
    free_audio_buffer(buf);
    
    return EXIT_SUCCESS;
}
