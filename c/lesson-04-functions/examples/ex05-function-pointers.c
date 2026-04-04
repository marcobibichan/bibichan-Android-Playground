/**
 * ex05-function-pointers.c
 * Purpose: Demonstrate function pointers and callbacks
 * Dependencies: stdio.h, stdlib.h, string.h
 * Compile: gcc -o ex05 ex05-function-pointers.c
 * Run: ./ex05
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function pointer typedef for comparison */
typedef int (*CompareFunc)(const void *, const void *);

/* Function pointer typedef for audio processing */
typedef void (*AudioProcessFunc)(int16_t *samples, size_t count);

/* Basic comparison functions */
int compare_int_asc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int compare_int_desc(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

/* Audio processing functions */
void audio_normalize(int16_t *samples, size_t count) {
    int16_t max_val = 0;
    for (size_t i = 0; i < count; i++) {
        if (abs(samples[i]) > max_val) {
            max_val = abs(samples[i]);
        }
    }
    if (max_val > 0) {
        float scale = 32767.0f / max_val;
        for (size_t i = 0; i < count; i++) {
            samples[i] = (int16_t)(samples[i] * scale);
        }
    }
}

void audio_reverse(int16_t *samples, size_t count) {
    for (size_t i = 0; i < count / 2; i++) {
        int16_t temp = samples[i];
        samples[i] = samples[count - 1 - i];
        samples[count - 1 - i] = temp;
    }
}

void audio_half_volume(int16_t *samples, size_t count) {
    for (size_t i = 0; i < count; i++) {
        samples[i] = samples[i] / 2;
    }
}

/* Generic audio processor using function pointer */
void process_audio(int16_t *samples, size_t count, AudioProcessFunc processor) {
    if (processor != NULL) {
        processor(samples, count);
    }
}

/* Callback context structure */
typedef struct {
    const char *filename;
    int total_frames;
    int current_frame;
} ProgressContext;

/* Progress callback type */
typedef void (*ProgressCallback)(const ProgressContext *ctx, void *userdata);

/* Simulate audio processing with progress callback */
void process_with_progress(const char *filename, 
                           ProgressCallback callback,
                           void *userdata) {
    ProgressContext ctx;
    ctx.filename = filename;
    ctx.total_frames = 100;
    
    for (int i = 0; i <= ctx.total_frames; i++) {
        ctx.current_frame = i;
        if (callback != NULL) {
            callback(&ctx, userdata);
        }
    }
}

/* Progress callback implementations */
void console_progress_callback(const ProgressContext *ctx, void *userdata) {
    if (ctx->current_frame % 10 == 0) {
        printf("Processing %s: %d/%d (%.0f%%)\n", 
               ctx->filename, 
               ctx->current_frame, 
               ctx->total_frames,
               (float)ctx->current_frame / ctx->total_frames * 100);
    }
}

void silent_callback(const ProgressContext *ctx, void *userdata) {
    /* Do nothing - silent processing */
}

/* Function pointer array */
int (*operations[])(int, int) = {
    NULL,  /* placeholder for 0 */
    NULL,  /* placeholder for 1 */
    NULL,  /* placeholder for 2 */
    NULL,  /* placeholder for 3 */
    NULL,  /* placeholder for 4 */
};

int add_op(int a, int b) { return a + b; }
int sub_op(int a, int b) { return a - b; }
int mul_op(int a, int b) { return a * b; }
int div_op(int a, int b) { return b != 0 ? a / b : 0; }

int main(void) {
    printf("=== Function Pointers for Sorting ===\n");
    
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int count = sizeof(arr) / sizeof(arr[0]);
    
    /* Sort ascending */
    qsort(arr, count, sizeof(int), compare_int_asc);
    printf("Ascending: ");
    for (int i = 0; i < count; i++) printf("%d ", arr[i]);
    printf("\n");
    
    /* Sort descending */
    qsort(arr, count, sizeof(int), compare_int_desc);
    printf("Descending: ");
    for (int i = 0; i < count; i++) printf("%d ", arr[i]);
    printf("\n\n");
    
    printf("=== Function Pointers for Audio Processing ===\n");
    
    int16_t samples[] = {1000, 2000, 3000, 4000, 5000};
    size_t sample_count = sizeof(samples) / sizeof(samples[0]);
    
    /* Process with different functions */
    AudioProcessFunc processors[] = {audio_normalize, audio_reverse, audio_half_volume};
    const char *names[] = {"Normalize", "Reverse", "Half Volume"};
    
    for (int i = 0; i < 3; i++) {
        /* Reset samples */
        int16_t test_samples[] = {1000, 2000, 3000, 4000, 5000};
        printf("%s: ", names[i]);
        process_audio(test_samples, sample_count, processors[i]);
        for (size_t j = 0; j < sample_count; j++) {
            printf("%d ", test_samples[j]);
        }
        printf("\n");
    }
    printf("\n");
    
    printf("=== Callback Functions ===\n");
    process_with_progress("audio.flac", console_progress_callback, NULL);
    printf("\n");
    
    printf("=== Operation Table ===\n");
    operations[1] = add_op;
    operations[2] = sub_op;
    operations[3] = mul_op;
    operations[4] = div_op;
    
    int a = 10, b = 3;
    printf("add(%d, %d) = %d\n", a, b, operations[1](a, b));
    printf("sub(%d, %d) = %d\n", a, b, operations[2](a, b));
    printf("mul(%d, %d) = %d\n", a, b, operations[3](a, b));
    printf("div(%d, %d) = %d\n", a, b, operations[4](a, b));
    
    return 0;
}
