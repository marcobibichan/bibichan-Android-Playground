// File: const_volatile.c
// Purpose: Demonstrate const and volatile type qualifiers
// Compile: gcc const_volatile.c -o const_volatile
// Run:     ./const_volatile

#include <stdio.h>
#include <stdint.h>
#include <signal.h>

// const: read-only variables
const int MAX_SAMPLE_RATE = 192000;
const char *CODEC_NAME = "FLAC";

// volatile: may change unexpectedly
volatile sig_atomic_t interrupted = 0;

// Signal handler (can be called asynchronously)
void signal_handler(int sig) {
    (void)sig;
    interrupted = 1;  // Must be volatile for signal safety
}

// const pointer vs pointer to const
void demonstrate_pointers(void) {
    int value = 42;
    int other = 100;
    
    // Pointer to const: cannot modify pointed value
    const int *ptr_to_const = &value;
    // *ptr_to_const = 50;  // Error: cannot modify
    ptr_to_const = &other;   // OK: can change pointer
    
    // Const pointer: cannot change pointer
    int * const const_ptr = &value;
    *const_ptr = 50;         // OK: can modify value
    // const_ptr = &other;   // Error: cannot change pointer
    
    // Const pointer to const: cannot change either
    const int * const const_ptr_to_const = &value;
    // *const_ptr_to_const = 50;  // Error
    // const_ptr_to_const = &other;  // Error
    
    (void)ptr_to_const;
    (void)const_ptr_to_const;
}

// const in function parameters
int process_audio(const int16_t *samples, size_t count) {
    // samples[0] = 0;  // Error: cannot modify
    int sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += samples[i];  // OK: can read
    }
    return sum;
}

// const return value
const char *get_codec_name(void) {
    return CODEC_NAME;  // Returns pointer to const string
}

int main(void) {
    printf("=== const Demo ===\n\n");
    
    // const variables
    printf("Max sample rate: %d\n", MAX_SAMPLE_RATE);
    printf("Codec name: %s\n", CODEC_NAME);
    // MAX_SAMPLE_RATE = 44100;  // Error: cannot modify const
    
    // const arrays
    const int channels[] = {1, 2, 6, 8};
    printf("Channel counts: %d, %d, %d, %d\n", 
           channels[0], channels[1], channels[2], channels[3]);
    
    printf("\n=== volatile Demo ===\n\n");
    
    // Setup signal handler
    signal(SIGINT, signal_handler);
    
    printf("Running... Press Ctrl+C to interrupt\n");
    
    // Simulate processing loop
    for (int i = 0; i < 10 && !interrupted; i++) {
        printf("Processing sample %d\n", i);
    }
    
    if (interrupted) {
        printf("Interrupted by signal!\n");
    }
    
    printf("\n=== Pointer const Demo ===\n\n");
    demonstrate_pointers();
    printf("Pointer demonstrations completed\n");
    
    return 0;
}
