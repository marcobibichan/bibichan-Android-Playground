/*
 * Purpose: Complete audio configuration structure for Hi-Res player
 * Dependencies: stdio.h, stdint.h, string.h, stdlib.h
 * Compile: gcc -o ex05 ex05-audio-config.c -Wall -Wextra
 * Run: ./ex05
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Audio format enumeration */
typedef enum {
    AUDIO_FORMAT_PCM = 0,
    AUDIO_FORMAT_FLOAT = 1,
    AUDIO_FORMAT_DSD = 2,
    AUDIO_FORMAT_COMPRESSED = 3
} AudioFormat;

/* Sample rate enumeration */
typedef enum {
    SAMPLE_RATE_44100 = 0,
    SAMPLE_RATE_48000 = 1,
    SAMPLE_RATE_88200 = 2,
    SAMPLE_RATE_96000 = 3,
    SAMPLE_RATE_176400 = 4,
    SAMPLE_RATE_192000 = 5,
    SAMPLE_RATE_352800 = 6,
    SAMPLE_RATE_384000 = 7
} SampleRateCode;

/* Channel configuration */
typedef enum {
    CHANNEL_MONO = 1,
    CHANNEL_STEREO = 2,
    CHANNEL_2POINT1 = 3,
    CHANNEL_QUAD = 4,
    CHANNEL_5POINT1 = 6,
    CHANNEL_7POINT1 = 8
} ChannelConfig;

/* Bit depth options */
typedef enum {
    BIT_DEPTH_16 = 16,
    BIT_DEPTH_24 = 24,
    BIT_DEPTH_32 = 32,
    BIT_DEPTH_64 = 64
} BitDepth;

/* Complete audio configuration structure */
typedef struct AudioConfig {
    /* Basic format info */
    AudioFormat format;
    uint32_t sample_rate;
    ChannelConfig channels;
    BitDepth bit_depth;
    
    /* Advanced settings */
    uint32_t buffer_size_ms;      /* Buffer duration in milliseconds */
    uint32_t target_latency_ms;   /* Target output latency */
    uint8_t exclusive_mode;       /* Exclusive audio mode flag */
    uint8_t dither_enabled;       /* Dithering for bit depth conversion */
    uint8_t resample_quality;     /* 0=fast, 1=medium, 2=best */
    uint8_t reserved;             /* Padding/alignment */
    
    /* Volume and gain */
    float volume;                 /* 0.0 to 1.0 */
    float preamp_db;              /* Preamp gain in dB */
    
    /* DSP settings */
    struct {
        uint8_t enabled;
        float cutoff_hz;
        float resonance;
    } lowpass_filter;
    
    struct {
        uint8_t enabled;
        float frequency_hz;
        float gain_db;
        float Q;
    } equalizer_bands[10];       /* 10-band equalizer */
    
    /* Output device info */
    char device_name[256];
    char device_id[64];
    
    /* Callback pointers (for audio pipeline) */
    int (*on_buffer_ready)(struct AudioConfig* config, void* buffer, size_t size);
    void (*on_error)(struct AudioConfig* config, const char* message);
    
} AudioConfig;

/* Function to initialize default configuration */
void audio_config_init_default(AudioConfig* config) {
    if (config == NULL) return;
    
    memset(config, 0, sizeof(AudioConfig));
    
    /* Default Hi-Res settings */
    config->format = AUDIO_FORMAT_PCM;
    config->sample_rate = 192000;
    config->channels = CHANNEL_STEREO;
    config->bit_depth = BIT_DEPTH_24;
    
    /* Buffer settings */
    config->buffer_size_ms = 500;     /* 500ms buffer */
    config->target_latency_ms = 10;   /* 10ms target latency */
    config->exclusive_mode = 1;       /* Enable exclusive mode */
    config->dither_enabled = 1;       /* Enable dithering */
    config->resample_quality = 2;     /* Best quality */
    
    /* Volume */
    config->volume = 1.0f;
    config->preamp_db = 0.0f;
    
    /* DSP defaults */
    config->lowpass_filter.enabled = 0;
    config->lowpass_filter.cutoff_hz = 20000.0f;
    
    /* Default device */
    strcpy(config->device_name, "Default Audio Device");
    strcpy(config->device_id, "default");
}

/* Function to calculate bytes per sample */
uint32_t audio_config_bytes_per_sample(const AudioConfig* config) {
    if (config == NULL) return 0;
    
    switch (config->format) {
        case AUDIO_FORMAT_PCM:
            return config->bit_depth / 8;
        case AUDIO_FORMAT_FLOAT:
            return 4;  /* 32-bit float */
        case AUDIO_FORMAT_DSD:
            return 1;  /* DSD is 1-bit, but stored as bytes */
        default:
            return 0;
    }
}

/* Function to calculate buffer size in bytes */
size_t audio_config_buffer_size_bytes(const AudioConfig* config) {
    if (config == NULL) return 0;
    
    uint32_t bytes_per_sample = audio_config_bytes_per_sample(config);
    uint32_t samples_per_ms = config->sample_rate / 1000;
    size_t total_samples = samples_per_ms * config->buffer_size_ms * config->channels;
    
    return total_samples * bytes_per_sample;
}

/* Function to print configuration */
void audio_config_print(const AudioConfig* config) {
    if (config == NULL) {
        printf("AudioConfig is NULL\n");
        return;
    }
    
    printf("=== Audio Configuration ===\n");
    printf("Format: ");
    switch (config->format) {
        case AUDIO_FORMAT_PCM: printf("PCM\n"); break;
        case AUDIO_FORMAT_FLOAT: printf("Float\n"); break;
        case AUDIO_FORMAT_DSD: printf("DSD\n"); break;
        default: printf("Unknown\n");
    }
    printf("Sample Rate: %u Hz\n", config->sample_rate);
    printf("Channels: %d\n", config->channels);
    printf("Bit Depth: %d bits\n", config->bit_depth);
    printf("Buffer Size: %u ms (%zu bytes)\n", 
           config->buffer_size_ms, audio_config_buffer_size_bytes(config));
    printf("Target Latency: %u ms\n", config->target_latency_ms);
    printf("Exclusive Mode: %s\n", config->exclusive_mode ? "Enabled" : "Disabled");
    printf("Dithering: %s\n", config->dither_enabled ? "Enabled" : "Disabled");
    printf("Volume: %.2f\n", config->volume);
    printf("Preamp: %.1f dB\n", config->preamp_db);
    printf("Device: %s (%s)\n", config->device_name, config->device_id);
}

/* Sample callback function */
int on_buffer_ready_callback(AudioConfig* config, void* buffer, size_t size) {
    printf("Buffer ready: %zu bytes\n", size);
    return 0;
}

/* Sample error callback function */
void on_error_callback(AudioConfig* config, const char* message) {
    fprintf(stderr, "Audio Error: %s\n", message);
}

int main(void) {
    printf("=== Audio Configuration Demo ===\n\n");
    
    /* Create and initialize configuration */
    AudioConfig config;
    audio_config_init_default(&config);
    
    /* Set callbacks */
    config.on_buffer_ready = on_buffer_ready_callback;
    config.on_error = on_error_callback;
    
    /* Print configuration */
    audio_config_print(&config);
    
    /* Modify settings */
    printf("\n=== Modifying Configuration ===\n");
    config.sample_rate = 384000;  /* Ultra Hi-Res */
    config.bit_depth = BIT_DEPTH_32;
    config.volume = 0.8f;
    config.preamp_db = -3.0f;
    
    /* Enable lowpass filter */
    config.lowpass_filter.enabled = 1;
    config.lowpass_filter.cutoff_hz = 40000.0f;  /* 40kHz for ultrasonic content */
    
    /* Configure equalizer */
    config.equalizer_bands[0].enabled = 1;
    config.equalizer_bands[0].frequency_hz = 32.0f;
    config.equalizer_bands[0].gain_db = 2.0f;
    config.equalizer_bands[0].Q = 1.0f;
    
    /* Print modified configuration */
    printf("\n=== Modified Configuration ===\n");
    audio_config_print(&config);
    
    /* Calculate sizes */
    printf("\n=== Size Calculations ===\n");
    printf("sizeof(AudioConfig): %zu bytes\n", sizeof(AudioConfig));
    printf("Bytes per sample: %u\n", audio_config_bytes_per_sample(&config));
    printf("Buffer size: %zu bytes\n", audio_config_buffer_size_bytes(&config));
    
    /* Test callback */
    printf("\n=== Testing Callbacks ===\n");
    uint8_t test_buffer[1024] = {0};
    config.on_buffer_ready(&config, test_buffer, sizeof(test_buffer));
    config.on_error(&config, "Test error message");
    
    return 0;
}
