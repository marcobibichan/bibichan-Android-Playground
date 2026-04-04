/*
 * Purpose: Demonstrate union usage for audio sample type flexibility
 * Dependencies: stdio.h, stdint.h, string.h
 * Compile: gcc -o ex03 ex03-unions.c -Wall -Wextra
 * Run: ./ex03
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Union for storing different sample formats */
typedef union AudioSample {
    int8_t i8;          /* 8-bit signed integer */
    int16_t i16;        /* 16-bit signed integer (CD quality) */
    int32_t i32;        /* 32-bit signed integer (Hi-Res) */
    float f32;          /* 32-bit float (professional) */
    double f64;         /* 64-bit double (mastering) */
    uint8_t bytes[8];   /* Raw byte access */
} AudioSample;

/* Tagged union - includes type indicator */
typedef struct {
    enum SampleType {
        SAMPLE_INT8,
        SAMPLE_INT16,
        SAMPLE_INT32,
        SAMPLE_FLOAT32,
        SAMPLE_FLOAT64
    } type;
    AudioSample value;
} TaggedSample;

/* Union for WAV file format chunk */
typedef union {
    struct {
        uint16_t audio_format;   /* 1 = PCM, 3 = IEEE float */
        uint16_t num_channels;
        uint32_t sample_rate;
        uint32_t byte_rate;
        uint16_t block_align;
        uint16_t bits_per_sample;
    } pcm;
    uint8_t raw[24];             /* Raw byte access for parsing */
} WavFormatChunk;

/* Function to print sample based on type */
void print_tagged_sample(const TaggedSample* ts) {
    switch (ts->type) {
        case SAMPLE_INT8:
            printf("INT8: %d\n", ts->value.i8);
            break;
        case SAMPLE_INT16:
            printf("INT16: %d\n", ts->value.i16);
            break;
        case SAMPLE_INT32:
            printf("INT32: %d\n", ts->value.i32);
            break;
        case SAMPLE_FLOAT32:
            printf("FLOAT32: %.6f\n", ts->value.f32);
            break;
        case SAMPLE_FLOAT64:
            printf("FLOAT64: %.15f\n", ts->value.f64);
            break;
        default:
            printf("Unknown sample type\n");
    }
}

/* Function to convert sample to float (normalized to -1.0 to 1.0) */
double sample_to_float(const TaggedSample* ts) {
    switch (ts->type) {
        case SAMPLE_INT8:
            return ts->value.i8 / 128.0;
        case SAMPLE_INT16:
            return ts->value.i16 / 32768.0;
        case SAMPLE_INT32:
            return ts->value.i32 / 2147483648.0;
        case SAMPLE_FLOAT32:
            return (double)ts->value.f32;
        case SAMPLE_FLOAT64:
            return ts->value.f64;
        default:
            return 0.0;
    }
}

int main(void) {
    printf("=== Union Size Demonstration ===\n");
    printf("sizeof(AudioSample): %zu bytes\n", sizeof(AudioSample));
    printf("sizeof(TaggedSample): %zu bytes\n", sizeof(TaggedSample));
    printf("sizeof(WavFormatChunk): %zu bytes\n", sizeof(WavFormatChunk));
    
    /* Using basic union */
    printf("\n=== Basic Union Usage ===\n");
    AudioSample sample;
    
    /* Store as 16-bit integer */
    sample.i16 = 16384;
    printf("Stored as INT16: %d\n", sample.i16);
    printf("Bytes: ");
    for (int i = 0; i < 2; i++) {
        printf("%02x ", sample.bytes[i]);
    }
    printf("\n");
    
    /* Store as float (overwrites i16) */
    sample.f32 = 0.5f;
    printf("Stored as FLOAT32: %.6f\n", sample.f32);
    printf("Bytes: ");
    for (int i = 0; i < 4; i++) {
        printf("%02x ", sample.bytes[i]);
    }
    printf("\n");
    
    /* Using tagged union for type safety */
    printf("\n=== Tagged Union Usage ===\n");
    TaggedSample ts1 = {.type = SAMPLE_INT16, .value.i16 = -8192};
    TaggedSample ts2 = {.type = SAMPLE_FLOAT32, .value.f32 = 0.25f};
    TaggedSample ts3 = {.type = SAMPLE_INT32, .value.i32 = 536870912};
    
    print_tagged_sample(&ts1);
    print_tagged_sample(&ts2);
    print_tagged_sample(&ts3);
    
    /* Converting to normalized float */
    printf("\n=== Normalized Values ===\n");
    printf("ts1 normalized: %.6f\n", sample_to_float(&ts1));
    printf("ts2 normalized: %.6f\n", sample_to_float(&ts2));
    printf("ts3 normalized: %.6f\n", sample_to_float(&ts3));
    
    /* WAV format chunk example */
    printf("\n=== WAV Format Chunk ===\n");
    WavFormatChunk wav;
    wav.pcm.audio_format = 1;      /* PCM */
    wav.pcm.num_channels = 2;      /* Stereo */
    wav.pcm.sample_rate = 192000;  /* Hi-Res */
    wav.pcm.byte_rate = 192000 * 2 * 3;  /* SampleRate * Channels * BytesPerSample */
    wav.pcm.block_align = 2 * 3;   /* Channels * BytesPerSample */
    wav.pcm.bits_per_sample = 24;  /* 24-bit */
    
    printf("WAV Format:\n");
    printf("  Audio Format: %d (PCM)\n", wav.pcm.audio_format);
    printf("  Channels: %d\n", wav.pcm.num_channels);
    printf("  Sample Rate: %d Hz\n", wav.pcm.sample_rate);
    printf("  Byte Rate: %d bytes/sec\n", wav.pcm.byte_rate);
    printf("  Block Align: %d bytes\n", wav.pcm.block_align);
    printf("  Bits/Sample: %d\n", wav.pcm.bits_per_sample);
    
    /* Raw byte access */
    printf("  Raw bytes: ");
    for (int i = 0; i < 24; i++) {
        printf("%02x ", wav.raw[i]);
    }
    printf("\n");
    
    return 0;
}
