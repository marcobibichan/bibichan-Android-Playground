/*
 * Purpose: Parse WAV file header and extract audio format information
 * Dependencies: stdio.h, stdint.h, string.h
 * Compile: gcc -o ex02 ex02-wav-header.c -Wall -Wextra
 * Run: ./ex02 audio.wav
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* WAV file header structure (packed for binary compatibility) */
#pragma pack(push, 1)
typedef struct {
    /* RIFF chunk descriptor */
    char riff_id[4];        /* "RIFF" */
    uint32_t file_size;     /* File size - 8 bytes */
    char wave_id[4];        /* "WAVE" */
    
    /* Format chunk */
    char fmt_id[4];         /* "fmt " */
    uint32_t fmt_size;      /* Format chunk size (16 for PCM) */
    uint16_t audio_format;  /* Audio format (1 = PCM, 3 = IEEE float) */
    uint16_t num_channels;  /* Number of channels */
    uint32_t sample_rate;   /* Sample rate in Hz */
    uint32_t byte_rate;     /* Bytes per second */
    uint16_t block_align;   /* Block alignment */
    uint16_t bits_per_sample; /* Bits per sample */
    
    /* Data chunk (may have other chunks before this) */
    char data_id[4];        /* "data" */
    uint32_t data_size;     /* Data size in bytes */
} WavHeader;
#pragma pack(pop)

/* Audio format names */
const char* get_audio_format_name(uint16_t format) {
    switch (format) {
        case 1:  return "PCM (integer)";
        case 2:  return "Microsoft ADPCM";
        case 3:  return "IEEE float";
        case 6:  return "ITU G.711 A-law";
        case 7:  return "ITU G.711 µ-law";
        case 65534: return "WAVE_FORMAT_EXTENSIBLE";
        default: return "Unknown";
    }
}

/* Calculate duration in seconds */
double calculate_duration(uint32_t data_size, uint32_t sample_rate, 
                          uint16_t channels, uint16_t bits_per_sample) {
    uint32_t bytes_per_sample = bits_per_sample / 8;
    uint32_t total_samples = data_size / (channels * bytes_per_sample);
    return (double)total_samples / sample_rate;
}

/* Check if format is Hi-Res */
int is_hires_format(uint32_t sample_rate, uint16_t bits_per_sample) {
    return (sample_rate >= 96000 || bits_per_sample >= 24);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <wav_file>\n", argv[0]);
        return 1;
    }
    
    const char* filename = argv[1];
    
    /* Open file in binary mode */
    FILE* wav_file = fopen(filename, "rb");
    if (wav_file == NULL) {
        perror("Failed to open WAV file");
        return 1;
    }
    
    /* Get file size */
    if (fseek(wav_file, 0, SEEK_END) != 0) {
        perror("Failed to seek to end");
        fclose(wav_file);
        return 1;
    }
    
    long file_size = ftell(wav_file);
    if (file_size < 0) {
        perror("Failed to get file size");
        fclose(wav_file);
        return 1;
    }
    
    /* Seek back to beginning */
    rewind(wav_file);
    
    printf("File: %s\n", filename);
    printf("File size: %ld bytes (%.2f MB)\n", file_size, file_size / (1024.0 * 1024.0));
    
    /* Read header */
    WavHeader header;
    size_t read_count = fread(&header, sizeof(WavHeader), 1, wav_file);
    if (read_count != 1) {
        if (feof(wav_file)) {
            fprintf(stderr, "File too small to be a valid WAV file\n");
        } else {
            perror("Failed to read WAV header");
        }
        fclose(wav_file);
        return 1;
    }
    
    /* Validate RIFF header */
    if (memcmp(header.riff_id, "RIFF", 4) != 0) {
        fprintf(stderr, "Invalid WAV file: RIFF header not found\n");
        fclose(wav_file);
        return 1;
    }
    
    /* Validate WAVE format */
    if (memcmp(header.wave_id, "WAVE", 4) != 0) {
        fprintf(stderr, "Invalid WAV file: WAVE format not found\n");
        fclose(wav_file);
        return 1;
    }
    
    /* Validate fmt chunk */
    if (memcmp(header.fmt_id, "fmt ", 4) != 0) {
        fprintf(stderr, "Invalid WAV file: fmt chunk not found\n");
        fclose(wav_file);
        return 1;
    }
    
    /* Validate data chunk */
    if (memcmp(header.data_id, "data", 4) != 0) {
        fprintf(stderr, "Warning: data chunk not at expected position\n");
        /* May have other chunks, need to search for data chunk */
    }
    
    /* Print header information */
    printf("\n=== WAV Header Information ===\n");
    printf("Audio Format: %s (%u)\n", get_audio_format_name(header.audio_format), 
           header.audio_format);
    printf("Channels: %u\n", header.num_channels);
    printf("Sample Rate: %u Hz\n", header.sample_rate);
    printf("Bits per Sample: %u\n", header.bits_per_sample);
    printf("Byte Rate: %u bytes/sec\n", header.byte_rate);
    printf("Block Align: %u bytes\n", header.block_align);
    
    /* Calculate and print duration */
    double duration = calculate_duration(header.data_size, header.sample_rate,
                                          header.num_channels, header.bits_per_sample);
    printf("Duration: %.2f seconds (%.2f minutes)\n", duration, duration / 60.0);
    
    /* Calculate bitrate */
    uint32_t bitrate = header.byte_rate * 8;
    printf("Bitrate: %u kbps\n", bitrate / 1000);
    
    /* Check Hi-Res status */
    printf("\n=== Format Analysis ===\n");
    if (is_hires_format(header.sample_rate, header.bits_per_sample)) {
        printf("✓ Hi-Res Audio format detected!\n");
        if (header.sample_rate >= 192000) {
            printf("  - Ultra Hi-Res sample rate (%u Hz)\n", header.sample_rate);
        }
        if (header.bits_per_sample >= 24) {
            printf("  - High bit depth (%u-bit)\n", header.bits_per_sample);
        }
    } else {
        printf("✗ Standard definition audio\n");
        printf("  - Sample rate: %u Hz\n", header.sample_rate);
        printf("  - Bit depth: %u-bit\n", header.bits_per_sample);
    }
    
    /* Validate header consistency */
    printf("\n=== Header Validation ===\n");
    uint32_t expected_byte_rate = header.sample_rate * header.num_channels * 
                                   (header.bits_per_sample / 8);
    uint16_t expected_block_align = header.num_channels * (header.bits_per_sample / 8);
    
    int valid = 1;
    if (header.byte_rate != expected_byte_rate) {
        printf("⚠ Byte rate mismatch: expected %u, got %u\n", 
               expected_byte_rate, header.byte_rate);
        valid = 0;
    }
    if (header.block_align != expected_block_align) {
        printf("⚠ Block align mismatch: expected %u, got %u\n",
               expected_block_align, header.block_align);
        valid = 0;
    }
    if (valid) {
        printf("✓ Header values are consistent\n");
    }
    
    fclose(wav_file);
    return 0;
}
