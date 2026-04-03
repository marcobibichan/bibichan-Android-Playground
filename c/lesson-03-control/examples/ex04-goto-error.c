/*
 * ex04-goto-error.c - Goto for error handling (FFmpeg style)
 * Compile: gcc -Wall -Wextra -o goto_error ex04-goto-error.c
 * Run: ./goto_error
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Simulated FFmpeg structures
typedef struct AVFormatContext {
    char filename[256];
    int nb_streams;
    void *priv_data;
} AVFormatContext;

typedef struct AVCodecContext {
    int codec_id;
    int sample_rate;
    int channels;
    void *priv_data;
} AVCodecContext;

typedef struct AVFrame {
    int nb_samples;
    int format;
    void *data[8];
} AVFrame;

typedef struct AVPacket {
    int size;
    void *data;
} AVPacket;

// Simulated error codes
#define AVERROR_EOF (-1)
#define AVERROR(E) (-(E))
#define AVERROR_UNKNOWN (-2)

// Simulated functions
AVFormatContext* avformat_alloc_context(void) {
    AVFormatContext *s = calloc(1, sizeof(AVFormatContext));
    return s;
}

void avformat_close_input(AVFormatContext **s) {
    if (s && *s) {
        free((*s)->priv_data);
        free(*s);
        *s = NULL;
    }
}

AVCodecContext* avcodec_alloc_context(void) {
    AVCodecContext *c = calloc(1, sizeof(AVCodecContext));
    return c;
}

void avcodec_free_context(AVCodecContext **c) {
    if (c && *c) {
        free((*c)->priv_data);
        free(*c);
        *c = NULL;
    }
}

AVFrame* av_frame_alloc(void) {
    AVFrame *f = calloc(1, sizeof(AVFrame));
    return f;
}

void av_frame_free(AVFrame **f) {
    if (f && *f) {
        for (int i = 0; i < 8; i++) {
            free((*f)->data[i]);
        }
        free(*f);
        *f = NULL;
    }
}

AVPacket* av_packet_alloc(void) {
    AVPacket *p = calloc(1, sizeof(AVPacket));
    return p;
}

void av_packet_free(AVPacket **p) {
    if (p && *p) {
        free((*p)->data);
        free(*p);
        *p = NULL;
    }
}

// Simulated avformat_open_input
int avformat_open_input(AVFormatContext **ps, const char *filename) {
    if (!filename || !ps) return AVERROR(EINVAL);
    
    // Simulate file not found
    if (strstr(filename, "nonexistent")) {
        return AVERROR(ENOENT);
    }
    
    // Simulate permission denied
    if (strstr(filename, "protected")) {
        return AVERROR(EACCES);
    }
    
    (*ps)->nb_streams = 2;
    strncpy((*ps)->filename, filename, sizeof((*ps)->filename) - 1);
    return 0;
}

// Simulated avcodec_open2
int avcodec_open2(AVCodecContext *c, int codec_id) {
    if (!c) return AVERROR(EINVAL);
    
    // Simulate unsupported codec
    if (codec_id > 10) {
        return AVERROR_UNKNOWN;
    }
    
    c->codec_id = codec_id;
    c->sample_rate = 192000;
    c->channels = 2;
    return 0;
}

// ============================================================================
// Example 1: Basic error handling with goto (FFmpeg style)
// ============================================================================
int decode_audio_file_basic(const char *filename) {
    AVFormatContext *fmt_ctx = NULL;
    AVCodecContext *codec_ctx = NULL;
    AVFrame *frame = NULL;
    int ret;
    
    printf("\n=== Basic Goto Error Handling ===\n");
    printf("Opening file: %s\n", filename);
    
    // Allocate format context
    fmt_ctx = avformat_alloc_context();
    if (!fmt_ctx) {
        ret = AVERROR(ENOMEM);
        goto fail;
    }
    
    // Open input file
    ret = avformat_open_input(&fmt_ctx, filename);
    if (ret < 0) {
        goto fail_format;
    }
    
    // Allocate codec context
    codec_ctx = avcodec_alloc_context();
    if (!codec_ctx) {
        ret = AVERROR(ENOMEM);
        goto fail_codec;
    }
    
    // Open codec
    ret = avcodec_open2(codec_ctx, 1);
    if (ret < 0) {
        goto fail_codec_open;
    }
    
    // Allocate frame
    frame = av_frame_alloc();
    if (!frame) {
        ret = AVERROR(ENOMEM);
        goto fail_frame;
    }
    
    // Success! Process audio...
    printf("Successfully opened file: %s\n", filename);
    printf("  Sample rate: %d Hz\n", codec_ctx->sample_rate);
    printf("  Channels: %d\n", codec_ctx->channels);
    
    // Normal cleanup path
    ret = 0;
    goto success;
    
// Error cleanup labels (in reverse order of allocation)
fail_frame:
    // Nothing to clean for frame in error path
fail_codec_open:
    // Codec context will be freed in fail_codec
fail_codec:
    avcodec_free_context(&codec_ctx);
fail_format:
    avformat_close_input(&fmt_ctx);
fail:
    printf("Error: %d\n", ret);
    return ret;
    
success:
    // Success cleanup (same order as error, but after success message)
    av_frame_free(&frame);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);
    return 0;
}

// ============================================================================
// Example 2: Nested loops with goto for early exit
// ============================================================================
int find_audio_packet_multi_stream(AVFormatContext *fmt_ctx, 
                                    int target_stream,
                                    int max_packets) {
    printf("\n=== Nested Loop with Goto ===\n");
    
    AVPacket *pkt = av_packet_alloc();
    if (!pkt) return AVERROR(ENOMEM);
    
    int found = 0;
    
    // Simulate reading packets
    for (int i = 0; i < max_packets; i++) {
        // Simulate multiple streams
        for (int stream = 0; stream < fmt_ctx->nb_streams; stream++) {
            printf("Checking packet %d, stream %d\n", i, stream);
            
            // Found target!
            if (stream == target_stream && i > 2) {
                found = 1;
                goto found_packet;  // Exit both loops!
            }
        }
    }
    
    // Not found
    printf("Target stream %d not found after %d packets\n", 
           target_stream, max_packets);
    av_packet_free(&pkt);
    return -1;
    
found_packet:
    printf("Found target stream %d at packet %d\n", target_stream, max_packets);
    av_packet_free(&pkt);
    return 0;
}

// ============================================================================
// Example 3: State machine with goto
// ============================================================================
typedef enum {
    STATE_IDLE,
    STATE_INITIALIZING,
    STATE_READY,
    STATE_DECODING,
    STATE_ERROR,
    STATE_CLEANUP
} PlayerState;

void run_player_state_machine_goto(void) {
    printf("\n=== State Machine with Goto ===\n");
    
    PlayerState state = STATE_IDLE;
    int retry_count = 0;
    const int max_retries = 3;
    
    goto state_idle;
    
state_idle:
    printf("[IDLE] Waiting for start command...\n");
    state = STATE_INITIALIZING;
    goto state_initializing;
    
state_initializing:
    printf("[INIT] Initializing decoder...\n");
    
    // Simulate initialization failure
    if (retry_count < 2) {
        retry_count++;
        printf("[INIT] Initialization failed, retry %d/%d\n", 
               retry_count, max_retries);
        if (retry_count >= max_retries) {
            state = STATE_ERROR;
            goto state_error;
        }
        goto state_initializing;
    }
    
    printf("[INIT] Initialization successful\n");
    state = STATE_READY;
    goto state_ready;
    
state_ready:
    printf("[READY] Decoder ready, starting decode...\n");
    state = STATE_DECODING;
    goto state_decoding;
    
state_decoding:
    printf("[DECODING] Processing audio frames...\n");
    // Simulate decode completion
    printf("[DECODING] Decode complete\n");
    state = STATE_CLEANUP;
    goto state_cleanup;
    
state_error:
    printf("[ERROR] Fatal error occurred\n");
    state = STATE_CLEANUP;
    goto state_cleanup;
    
state_cleanup:
    printf("[CLEANUP] Releasing resources...\n");
    printf("[CLEANUP] Done\n");
}

// ============================================================================
// Example 4: Comparison with non-goto approach
// ============================================================================
int decode_without_goto(const char *filename) {
    printf("\n=== Without Goto (Nested Ifs) ===\n");
    
    AVFormatContext *fmt_ctx = NULL;
    AVCodecContext *codec_ctx = NULL;
    AVFrame *frame = NULL;
    int ret = 0;
    int success = 0;
    
    fmt_ctx = avformat_alloc_context();
    if (fmt_ctx) {
        ret = avformat_open_input(&fmt_ctx, filename);
        if (ret >= 0) {
            codec_ctx = avcodec_alloc_context();
            if (codec_ctx) {
                ret = avcodec_open2(codec_ctx, 1);
                if (ret >= 0) {
                    frame = av_frame_alloc();
                    if (frame) {
                        printf("Success!\n");
                        success = 1;
                        av_frame_free(&frame);
                    } else {
                        ret = AVERROR(ENOMEM);
                    }
                    avcodec_free_context(&codec_ctx);
                }
            } else {
                ret = AVERROR(ENOMEM);
            }
            avformat_close_input(&fmt_ctx);
        }
    } else {
        ret = AVERROR(ENOMEM);
    }
    
    return success ? 0 : ret;
}

int main(void) {
    printf("=== Goto for Error Handling Demo ===\n");
    
    // Test basic error handling
    decode_audio_file_basic("test.flac");
    decode_audio_file_basic("nonexistent.flac");
    decode_audio_file_basic("protected.flac");
    
    // Test nested loop exit
    AVFormatContext fmt = {.nb_streams = 3};
    find_audio_packet_multi_stream(&fmt, 1, 10);
    
    // Test state machine
    run_player_state_machine_goto();
    
    // Compare with non-goto
    decode_without_goto("test.flac");
    
    return EXIT_SUCCESS;
}
