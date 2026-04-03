// File: hello_args.c
// Purpose: Demonstrate command-line argument handling
// Compile: gcc hello_args.c -o hello_args
// Run:     ./hello_args arg1 arg2 arg3

#include <stdio.h>  // printf, fprintf
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

int main(int argc, char *argv[]) {
    // argc: argument count (includes program name)
    // argv: argument vector (array of strings)
    // argv[0]: program name (may be empty or path)
    // argv[1] to argv[argc-1]: command-line arguments
    // argv[argc]: always NULL (sentinel)
    
    printf("Program name: %s\n", argv[0]);
    printf("Argument count: %d\n", argc);
    
    // Loop through all arguments
    for (int i = 1; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    
    // Check if user provided enough arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <name>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    printf("Hello, %s!\n", argv[1]);
    
    return EXIT_SUCCESS;
}
