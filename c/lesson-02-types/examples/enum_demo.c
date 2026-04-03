// File: enum_demo.c
// Purpose: Demonstrate enumeration types for audio codecs
// Compile: gcc enum_demo.c -o enum_demo
// Run:     ./enum_demo

#include <stdio.h>
#include <stdint.h>

// Basic enumeration
enum audio_format {
    FORMAT_NONE = 0,
    FORMAT_MP3,
    FORMAT_AAC,
    FORMAT_FLAC,
    FORMAT_WAV,
    FORMAT_ALAC,
    FORMAT_DSD
};

// Enumeration with specific values
enum sample_rate {
    SAMPLE_RATE_44100  = 44100,
    SAMPLE_RATE_48000  = 48000,
    SAMPLE_RATE_96000  = 96000,
    SAMPLE_RATE_192000 = 192000,
    SAMPLE_RATE_384000 = 384000
};

// Enumeration for bit depth
enum bit_depth {
    BIT_DEPTH_16 = 16,
    BIT_DEPTH_24 = 24,
    BIT_DEPTH_32 = 32
};

// Typedeffed enumeration
typedef enum {
    CHANNEL_MONO   = 1,
    CHANNEL_STEREO = 2,
    CHANNEL_5_1    = 6,
    CHANNEL_7_1    = 8
} ChannelConfig;

// Function using enumeration
const char *get_format_name(enum audio_format fmt) {
    switch (fmt) {
        case FORMAT_MP3:  return "MP3";
        case FORMAT_AAC:  return "AAC";
        case FORMAT_FLAC: return "FLAC";
        case FORMAT_WAV:  return "WAV";
        case FORMAT_ALAC: return "ALAC";
        case FORMAT_DSD:  return "DSD";
        default:          return "Unknown";
    }
}

// Check if format is lossless
int is_lossless(enum audio_format fmt) {
    return fmt == FORMAT_FLAC || 
           fmt == FORMAT_WAV || 
           fmt == FORMAT_ALAC;
}

int main(void) {
    printf("=== Audio Format Enumeration ===\n\n");
    
    // Using enumeration
    enum audio_format my_format = FORMAT_FLAC;
    printf("Format: %s (value: %d)\n", 
           get_format_name(my_format), my_format);
    printf("Is lossless: %s\n", 
           is_lossless(my_format) ? "Yes" : "No");
    
    // Sample rate enumeration
    enum sample_rate rate = SAMPLE_RATE_192000;
    printf("\nSample rate: %d Hz\n", rate);
    
    // Bit depth enumeration
    enum bit_depth depth = BIT_DEPTH_24;
    printf("Bit depth: %d bits\n", depth);
    
    // Channel configuration
    ChannelConfig channels = CHANNEL_STEREO;
    printf("Channels: %d\n", channels);
    
    printf("\n=== All Formats ===\n\n");
    
    // Iterate through formats
    const enum audio_format formats[] = {
        FORMAT_MP3, FORMAT_AAC, FORMAT_FLAC, 
        FORMAT_WAV, FORMAT_ALAC, FORMAT_DSD
    };
    
    for (int i = 0; i < 6; i++) {
        enum audio_format fmt = formats[i];
        printf("%-6s: lossless=%s\n", 
               get_format_name(fmt),
               is_lossless(fmt) ? "Yes" : "No");
    }
    
    printf("\n=== Hi-Res Audio Config ===\n\n");
    
    // Combine enumerations
    struct {
        enum audio_format format;
        enum sample_rate rate;
        enum bit_depth depth;
        ChannelConfig channels;
    } hires_config = {
        .format = FORMAT_FLAC,
        .rate = SAMPLE_RATE_192000,
        .depth = BIT_DEPTH_24,
        .channels = CHANNEL_STEREO
    };
    
    printf("Hi-Res Configuration:\n");
    printf("  Format: %s\n", get_format_name(hires_config.format));
    printf("  Sample Rate: %d Hz\n", hires_config.rate);
    printf("  Bit Depth: %d bits\n", hires_config.depth);
    printf("  Channels: %d\n", hires_config.channels);
    
    // Calculate data rate
    uint64_t data_rate = (uint64_t)hires_config.rate * 
                         hires_config.depth * 
                         hires_config.channels / 8;
    printf("  Data Rate: %lu bytes/sec (%.2f MB/min)\n", 
           data_rate, data_rate * 60.0 / (1024 * 1024));
    
    return 0;
}
