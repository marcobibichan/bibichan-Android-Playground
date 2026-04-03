// File: hello_basic.c
// Purpose: Basic Hello World program demonstrating minimal C program structure
// Compile: gcc hello_basic.c -o hello_basic
// Run:     ./hello_basic

#include <stdio.h>  // Include standard I/O library for printf function

// Main function: program entry point
// Return type: int (exit status to operating system)
int main(void) {    // void indicates no parameters
    // printf: formatted print to stdout
    // "\n": newline character (line break)
    printf("Hello, World!\n");
    
    // Return 0 to indicate successful execution
    // Non-zero values indicate errors
    return 0;
}
