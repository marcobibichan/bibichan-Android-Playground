/*
 * Purpose: Process large audio files using buffered reading
 * Dependencies: stdio.h, stdint.h, stdlib.h, math.h
 * Compile: gcc -o ex03 ex03-large-file.c -Wall -Wextra -lm
 * Run: ./ex03 large_audio.pcm 24
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Buffer size for processing (64KB) */
#define BUFFER_SIZE (64 * 1024)

/* Audio sample structure for analysis */
typedef struct {
    int64_t total_samples;
    int64_t min_sample;
    int64_t max_sample;
    double sum_samples;
    double sum_squared;
} AudioAnalysis;

/* Initialize analysis structure */
void analysis_init(AudioAnalysis* analysis) {
    analysis->total_samples = 0;
    analysis->min_sample = INT32_MAX;
    analysis->max_sample = INT32_MIN;
    analysis->sum_samples = 0.0;
    analysis->sum_squared = 0.0;
}

/* Process a buffer of 24-bit signed samples (3 bytes each) */
void process_24bit_samples(const uint8_t* buffer, size_t bytes, 
                           AudioAnalysis* analysis) {
    size_t sample_count = bytes / 3;
    
    for (size_t i = 0; i < sample_count; i++) {
        /* Read 3 bytes and convert to 32-bit signed */
        int32_t sample = (int32_t)(
            ((int32_t)(int8_t)buffer[i * 3 + 2] << 16) |
            ((int32_t)buffer[i * 3 + 1] << 8) |
            ((int32_t)buffer[i * 3])
        );
        
        /* Update statistics */
        if (sample < analysis->min_sample) {
            analysis->min_sample = sample;
        }
        if (sample > analysis->max_sample) {
            analysis->max_sample = sample;
        }
        
        analysis->sum_samples += sample;
        analysis->sum_squared += (double)sample * sample;
        analysis->total_samples++;
    }
}

/* Process a buffer of 16-bit signed samples */
void process_16bit_samples(const int16_t* buffer, size_t sample_count,
                           AudioAnalysis* analysis) {
    for (size_t i = 0; i < sample_count; i++) {
        int16_t sample = buffer[i];
        
        if (sample < analysis->min_sample) {
            analysis->min_sample = sample;
        }
        if (sample > analysis->max_sample) {
            analysis->max_sample = sample;
        }
        
        analysis->sum_samples += sample;
        analysis->sum_squared += (double)sample * sample;
        analysis->total_samples++;
    }
}

/* Calculate RMS (Root Mean Square) level */
double calculate_rms(const AudioAnalysis* analysis) {
    if (analysis->total_samples == 0) return 0.0;
    double mean_squared = analysis->sum_squared / analysis->total_samples;
    return sqrt(mean_squared);
}

/* Calculate peak level in dB */
double calculate_peak_db(const AudioAnalysis* analysis, int bit_depth) {
    double max_value = (1 << (bit_depth - 1)) - 1;
    double peak = (analysis->max_sample > -analysis->min_sample) ?
                  analysis->max_sample : -analysis->min_sample;
    return 20.0 * log10(peak / max_value);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pcm_file> <bit_depth:16|24>\n", argv[0]);
        return 1;
    }
    
    const char* filename = argv[1];
    int bit_depth = atoi(argv[2]);
    
    if (bit_depth != 16 && bit_depth != 24) {
        fprintf(stderr, "Error: bit_depth must be 16 or 24\n");
        return 1;
    }
    
    /* Open file with custom buffer */
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open PCM file");
        return 1;
    }
    
    /* Set larger buffer for better performance */
    char* buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate buffer\n");
        fclose(file);
        return 1;
    }
    
    if (setvbuf(file, buffer, _IOFBF, BUFFER_SIZE) != 0) {
        fprintf(stderr, "Warning: Failed to set buffer, using default\n");
    }
    
    /* Get file size */
    fseek(file, 0, SEEK_END);
    int64_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    printf("Processing: %s\n", filename);
    printf("File size: %ld bytes (%.2f MB)\n", file_size, file_size / (1024.0 * 1024.0));
    printf("Bit depth: %d-bit\n", bit_depth);
    
    /* Initialize analysis */
    AudioAnalysis analysis;
    analysis_init(&analysis);
    
    /* Allocate read buffer */
    uint8_t* read_buffer = malloc(BUFFER_SIZE);
    if (read_buffer == NULL) {
        fprintf(stderr, "Failed to allocate read buffer\n");
        free(buffer);
        fclose(file);
        return 1;
    }
    
    /* Process file in chunks */
    size_t bytes_read;
    int64_t total_bytes = 0;
    int progress = 0;
    
    printf("\nProcessing: [");
    fflush(stdout);
    
    while ((bytes_read = fread(read_buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (bit_depth == 24) {
            process_24bit_samples(read_buffer, bytes_read, &analysis);
        } else {
            process_16bit_samples((int16_t*)read_buffer, bytes_read / 2, &analysis);
        }
        
        total_bytes += bytes_read;
        
        /* Update progress bar */
        int new_progress = (int)((double)total_bytes / file_size * 50);
        while (progress < new_progress) {
            printf("=");
            fflush(stdout);
            progress++;
        }
    }
    
    printf("] 100%%\n\n");
    
    /* Check for errors */
    if (ferror(file)) {
        perror("Error reading file");
        free(read_buffer);
        free(buffer);
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    /* Print analysis results */
    printf("=== Audio Analysis Results ===\n");
    printf("Total samples: %ld\n", analysis.total_samples);
    printf("Duration: %.2f seconds (assuming 192kHz)\n",
           (double)analysis.total_samples / 192000);
    
    printf("\n=== Level Analysis ===\n");
    printf("Min sample: %ld\n", analysis.min_sample);
    printf("Max sample: %ld\n", analysis.max_sample);
    printf("Peak-to-peak: %ld\n", analysis.max_sample - analysis.min_sample);
    
    double rms = calculate_rms(&analysis);
    double peak_db = calculate_peak_db(&analysis, bit_depth);
    
    printf("RMS level: %.2f\n", rms);
    printf("Peak level: %.2f dBFS\n", peak_db);
    
    /* Calculate dynamic range */
    double avg_level = analysis.sum_samples / analysis.total_samples;
    printf("Average level: %.2f\n", avg_level);
    
    /* Cleanup */
    free(read_buffer);
    free(buffer);
    
    return 0;
}
