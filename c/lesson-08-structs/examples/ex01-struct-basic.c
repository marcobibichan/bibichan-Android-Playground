/*
 * Purpose: Demonstrate basic structure declaration, initialization, and member access
 * Dependencies: stdio.h, string.h
 * Compile: gcc -o ex01 ex01-struct-basic.c -Wall -Wextra
 * Run: ./ex01
 */

#include <stdio.h>
#include <string.h>

/* Structure declaration (creates a new type) */
struct AudioHeader {
    int sample_rate;      /* Sampling frequency in Hz (e.g., 44100, 192000) */
    int channels;         /* Number of audio channels (1=mono, 2=stereo) */
    int bit_depth;        /* Bits per sample (16, 24, 32) */
    int duration_ms;      /* Duration in milliseconds */
};

/* Structure with flexible array member (C99 feature) */
struct AudioMetadata {
    char title[256];
    char artist[256];
    char album[256];
    int year;
    int track_number;
};

/* Typedef for convenience (creates alias) */
typedef struct {
    float left;
    float right;
} StereoSample;

/* Structure with nested structure */
struct AudioStream {
    struct AudioHeader header;      /* Nested structure member */
    struct AudioMetadata metadata;  /* Another nested structure */
    StereoSample* samples;          /* Pointer to sample buffer */
    size_t sample_count;            /* Number of samples */
};

int main(void) {
    /* Method 1: Designated initializer (C99, recommended) */
    struct AudioHeader header1 = {
        .sample_rate = 192000,
        .channels = 2,
        .bit_depth = 24,
        .duration_ms = 240000  /* 4 minutes */
    };
    
    /* Method 2: Positional initializer */
    struct AudioHeader header2 = {44100, 2, 16, 180000};
    
    /* Method 3: Zero initialization */
    struct AudioHeader header3 = {0};  /* All members set to 0 */
    
    /* Method 4: Member-by-member assignment */
    struct AudioHeader header4;
    header4.sample_rate = 96000;
    header4.channels = 2;
    header4.bit_depth = 24;
    header4.duration_ms = 300000;
    
    /* Accessing members with dot operator */
    printf("Header 1:\n");
    printf("  Sample Rate: %d Hz\n", header1.sample_rate);
    printf("  Channels: %d\n", header1.channels);
    printf("  Bit Depth: %d bits\n", header1.bit_depth);
    printf("  Duration: %d ms\n", header1.duration_ms);
    
    /* Structure assignment (copies all members) */
    struct AudioHeader header_copy = header1;
    printf("\nCopied header sample rate: %d Hz\n", header_copy.sample_rate);
    
    /* Using typedef'd structure */
    StereoSample sample = {-0.5f, 0.75f};
    printf("\nStereo sample: L=%.3f, R=%.3f\n", sample.left, sample.right);
    
    /* Nested structure access */
    struct AudioStream stream = {0};
    stream.header.sample_rate = 44100;
    stream.header.channels = 2;
    stream.header.bit_depth = 16;
    stream.header.duration_ms = 200000;
    
    /* Using strcpy for string members */
    strcpy(stream.metadata.title, "Hi-Res Audio Test");
    strcpy(stream.metadata.artist, "FFmpeg Tutorial");
    strcpy(stream.metadata.album, "Learning C");
    stream.metadata.year = 2024;
    stream.metadata.track_number = 1;
    
    /* Printing nested structure */
    printf("\nAudio Stream:\n");
    printf("  Title: %s\n", stream.metadata.title);
    printf("  Artist: %s\n", stream.metadata.artist);
    printf("  Sample Rate: %d Hz\n", stream.header.sample_rate);
    
    /* Calculating size */
    printf("\nStructure sizes:\n");
    printf("  sizeof(AudioHeader): %zu bytes\n", sizeof(struct AudioHeader));
    printf("  sizeof(AudioMetadata): %zu bytes\n", sizeof(struct AudioMetadata));
    printf("  sizeof(StereoSample): %zu bytes\n", sizeof(StereoSample));
    printf("  sizeof(AudioStream): %zu bytes\n", sizeof(struct AudioStream));
    
    return 0;
}
