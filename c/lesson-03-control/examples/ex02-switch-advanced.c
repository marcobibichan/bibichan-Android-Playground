/*
 * ex02-switch-advanced.c - Advanced switch statement usage
 * Compile: gcc -Wall -Wextra -o switch_advanced ex02-switch-advanced.c
 * Run: ./switch_advanced
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// FFmpeg codec ID simulation
typedef enum {
    AV_CODEC_ID_MP3 = 0,
    AV_CODEC_ID_AAC,
    AV_CODEC_ID_FLAC,
    AV_CODEC_ID_WAV,
    AV_CODEC_ID_OPUS,
    AV_CODEC_ID_VORBIS,
    AV_CODEC_ID_UNKNOWN
} CodecID;

// Audio sample format
typedef enum {
    SAMPLE_FMT_U8,      // unsigned 8-bit
    SAMPLE_FMT_S16,     // signed 16-bit
    SAMPLE_FMT_S32,     // signed 32-bit
    SAMPLE_FMT_FLT,     // float
    SAMPLE_FMT_DBL,     // double
    SAMPLE_FMT_S64,     // signed 64-bit
    SAMPLE_FMT_NB       // number of sample formats
} SampleFormat;

// Get bytes per sample for each format
int get_bytes_per_sample(SampleFormat fmt) {
    switch (fmt) {
        case SAMPLE_FMT_U8:
            return 1;
        case SAMPLE_FMT_S16:
            return 2;
        case SAMPLE_FMT_S32:
            return 4;
        case SAMPLE_FMT_FLT:
            return 4;
        case SAMPLE_FMT_DBL:
            return 8;
        case SAMPLE_FMT_S64:
            return 8;
        default:
            return 0;  // Unknown format
    }
}

// Get format name
const char* get_format_name(SampleFormat fmt) {
    // Using array for comparison (jump table optimization)
    static const char* names[] = {
        "U8", "S16", "S32", "FLT", "DBL", "S64"
    };
    
    if (fmt >= 0 && fmt < SAMPLE_FMT_NB) {
        return names[fmt];
    }
    return "Unknown";
}

// Process audio based on codec
void process_audio_codec(CodecID codec_id) {
    printf("Processing codec ID: %d\n", codec_id);
    
    switch (codec_id) {
        case AV_CODEC_ID_MP3:
            printf("  -> Initializing MP3 decoder\n");
            printf("  -> Setting up MPEG audio parser\n");
            // Fall-through intentional - share AAC setup
            // FALLTHROUGH
            
        case AV_CODEC_ID_AAC:
            printf("  -> Configuring lossy decoder parameters\n");
            printf("  -> Allocating synthesis buffer\n");
            break;
            
        case AV_CODEC_ID_FLAC:
            printf("  -> Initializing FLAC decoder\n");
            printf("  -> Setting up MDCT for lossless\n");
            printf("  -> Allocating 24-bit output buffer\n");
            break;
            
        case AV_CODEC_ID_WAV:
            printf("  -> PCM decoder (no decompression needed)\n");
            printf("  -> Direct buffer passthrough\n");
            break;
            
        case AV_CODEC_ID_OPUS:
        case AV_CODEC_ID_VORBIS:
            // Both use similar CELT-based processing
            printf("  -> Initializing CELT-based decoder\n");
            printf("  -> Setting up overlap-add\n");
            break;
            
        default:
            printf("  -> Unknown codec, using default handler\n");
            break;
    }
}

// State machine for audio decoding
typedef enum {
    DECODE_STATE_INIT,
    DECODE_STATE_HEADER,
    DECODE_STATE_DATA,
    DECODE_STATE_ERROR,
    DECODE_STATE_EOF
} DecodeState;

void run_decode_state_machine(void) {
    DecodeState state = DECODE_STATE_INIT;
    int iterations = 0;
    const int max_iterations = 10;
    
    printf("\n=== Decode State Machine ===\n");
    
    while (state != DECODE_STATE_EOF && iterations < max_iterations) {
        printf("State: ");
        switch (state) {
            case DECODE_STATE_INIT:
                printf("INIT -> ");
                printf("Initializing decoder...\n");
                state = DECODE_STATE_HEADER;
                break;
                
            case DECODE_STATE_HEADER:
                printf("HEADER -> ");
                printf("Reading frame header...\n");
                // Simulate: 70% data, 20% error, 10% EOF
                int rand_val = rand() % 10;
                if (rand_val < 7) {
                    state = DECODE_STATE_DATA;
                } else if (rand_val < 9) {
                    state = DECODE_STATE_ERROR;
                } else {
                    state = DECODE_STATE_EOF;
                }
                break;
                
            case DECODE_STATE_DATA:
                printf("DATA -> ");
                printf("Decoding audio samples...\n");
                state = DECODE_STATE_HEADER;  // Loop back for more
                break;
                
            case DECODE_STATE_ERROR:
                printf("ERROR -> ");
                printf("Error detected, attempting recovery...\n");
                state = DECODE_STATE_HEADER;  // Retry
                break;
                
            case DECODE_STATE_EOF:
                printf("EOF -> ");
                printf("End of stream reached.\n");
                break;
        }
        iterations++;
    }
}

// Benchmark switch vs if-else chain
void benchmark_comparison(void) {
    printf("\n=== Benchmark: switch vs if-else ===\n");
    
    const int iterations = 10000000;
    clock_t start, end;
    volatile int result = 0;
    
    // Benchmark switch
    start = clock();
    for (int i = 0; i < iterations; i++) {
        switch (i % 6) {
            case 0: result = 1; break;
            case 1: result = 2; break;
            case 2: result = 3; break;
            case 3: result = 4; break;
            case 4: result = 5; break;
            case 5: result = 6; break;
        }
    }
    end = clock();
    printf("Switch: %.3f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    // Benchmark if-else
    start = clock();
    for (int i = 0; i < iterations; i++) {
        int mod = i % 6;
        if (mod == 0) result = 1;
        else if (mod == 1) result = 2;
        else if (mod == 2) result = 3;
        else if (mod == 3) result = 4;
        else if (mod == 4) result = 5;
        else result = 6;
    }
    end = clock();
    printf("If-else: %.3f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main(void) {
    printf("=== Advanced Switch Statement Demo ===\n\n");
    
    srand(time(NULL));
    
    // Test sample format functions
    printf("Sample Format Information:\n");
    for (int i = 0; i < SAMPLE_FMT_NB; i++) {
        printf("  %s: %d bytes/sample\n", 
               get_format_name(i), get_bytes_per_sample(i));
    }
    
    // Test codec processing
    printf("\nCodec Processing:\n");
    process_audio_codec(AV_CODEC_ID_MP3);
    process_audio_codec(AV_CODEC_ID_FLAC);
    process_audio_codec(AV_CODEC_ID_OPUS);
    process_audio_codec(AV_CODEC_ID_UNKNOWN);
    
    // Run state machine
    run_decode_state_machine();
    
    // Benchmark
    benchmark_comparison();
    
    return EXIT_SUCCESS;
}
