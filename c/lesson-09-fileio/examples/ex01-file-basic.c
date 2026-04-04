/*
 * Purpose: Demonstrate basic file operations - open, read, write, close
 * Dependencies: stdio.h, stdlib.h, string.h
 * Compile: gcc -o ex01 ex01-file-basic.c -Wall -Wextra
 * Run: ./ex01
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure for audio configuration (for config file demo) */
typedef struct {
    int sample_rate;
    int channels;
    int bit_depth;
    float volume;
    char device_name[256];
} AudioConfig;

int main(void) {
    /* ========================================
     * Part 1: Writing a text configuration file
     * ======================================== */
    
    printf("=== Writing Configuration File ===\n");
    
    /* Open file for writing text */
    /* "w" mode: creates new file or truncates existing */
    FILE* config_file = fopen("audio_config.txt", "w");
    if (config_file == NULL) {
        perror("Failed to create config file");
        return 1;
    }
    
    /* Write configuration using fprintf */
    AudioConfig config = {
        .sample_rate = 192000,
        .channels = 2,
        .bit_depth = 24,
        .volume = 0.85f,
        .device_name = "USB DAC"
    };
    
    fprintf(config_file, "# Audio Configuration File\n");
    fprintf(config_file, "sample_rate=%d\n", config.sample_rate);
    fprintf(config_file, "channels=%d\n", config.channels);
    fprintf(config_file, "bit_depth=%d\n", config.bit_depth);
    fprintf(config_file, "volume=%.2f\n", config.volume);
    fprintf(config_file, "device=%s\n", config.device_name);
    
    /* Close file to flush and release resources */
    fclose(config_file);
    printf("Configuration file written successfully.\n\n");
    
    /* ========================================
     * Part 2: Reading the configuration file
     * ======================================== */
    
    printf("=== Reading Configuration File ===\n");
    
    /* Open file for reading text */
    /* "r" mode: file must exist */
    config_file = fopen("audio_config.txt", "r");
    if (config_file == NULL) {
        perror("Failed to open config file");
        return 1;
    }
    
    /* Read line by line using fgets */
    char line[512];
    AudioConfig read_config = {0};
    
    while (fgets(line, sizeof(line), config_file) != NULL) {
        /* Skip comments and empty lines */
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }
        
        /* Parse key=value pairs */
        char key[64], value[256];
        if (sscanf(line, "%63[^=]=%255[^\n]", key, value) == 2) {
            if (strcmp(key, "sample_rate") == 0) {
                read_config.sample_rate = atoi(value);
            } else if (strcmp(key, "channels") == 0) {
                read_config.channels = atoi(value);
            } else if (strcmp(key, "bit_depth") == 0) {
                read_config.bit_depth = atoi(value);
            } else if (strcmp(key, "volume") == 0) {
                read_config.volume = (float)atof(value);
            } else if (strcmp(key, "device") == 0) {
                strncpy(read_config.device_name, value, sizeof(read_config.device_name) - 1);
            }
        }
    }
    
    /* Check for errors (not just EOF) */
    if (ferror(config_file)) {
        perror("Error reading config file");
        fclose(config_file);
        return 1;
    }
    
    fclose(config_file);
    
    /* Print parsed configuration */
    printf("Parsed configuration:\n");
    printf("  Sample Rate: %d Hz\n", read_config.sample_rate);
    printf("  Channels: %d\n", read_config.channels);
    printf("  Bit Depth: %d\n", read_config.bit_depth);
    printf("  Volume: %.2f\n", read_config.volume);
    printf("  Device: %s\n\n", read_config.device_name);
    
    /* ========================================
     * Part 3: Binary file operations
     * ======================================== */
    
    printf("=== Binary File Operations ===\n");
    
    /* Write binary configuration */
    FILE* bin_file = fopen("audio_config.bin", "wb");
    if (bin_file == NULL) {
        perror("Failed to create binary file");
        return 1;
    }
    
    /* Write entire structure in one operation */
    size_t written = fwrite(&config, sizeof(AudioConfig), 1, bin_file);
    if (written != 1) {
        perror("Failed to write binary data");
        fclose(bin_file);
        return 1;
    }
    
    fclose(bin_file);
    printf("Binary config written: %zu bytes\n", sizeof(AudioConfig));
    
    /* Read binary configuration */
    bin_file = fopen("audio_config.bin", "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return 1;
    }
    
    AudioConfig bin_config;
    size_t read_count = fread(&bin_config, sizeof(AudioConfig), 1, bin_file);
    if (read_count != 1) {
        if (feof(bin_file)) {
            fprintf(stderr, "Unexpected end of file\n");
        } else if (ferror(bin_file)) {
            perror("Error reading binary file");
        }
        fclose(bin_file);
        return 1;
    }
    
    fclose(bin_file);
    
    printf("Binary config read:\n");
    printf("  Sample Rate: %d Hz\n", bin_config.sample_rate);
    printf("  Channels: %d\n", bin_config.channels);
    printf("  Bit Depth: %d\n", bin_config.bit_depth);
    printf("  Volume: %.2f\n", bin_config.volume);
    printf("  Device: %s\n", bin_config.device_name);
    
    return 0;
}
